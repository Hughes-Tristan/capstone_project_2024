// Fill out your copyright notice in the Description page of Project Settings.

// Enemy character class for enemy behavior
// Developer(s): Tristan Hughes, Joey Bertrand
// Last Updated: 11-29-24


/*#include "enemyPatrolCharacter.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "AttributeComponent.h"
#include "HealthBarComponent.h"
#include "damageComponent.h"
#include "waveManager.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"*/

/*
#include "enemyPatrolCharacter.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "developmentCharacterTH.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "waveManager.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Math/UnrealMathUtility.h"
#include "HealthBarComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "AttributeComponent.h"


// constructor used to initialize the damage component, wavemanager, and fix collision errors with the camera handling
AenemyPatrolCharacter::AenemyPatrolCharacter()
{

    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    
    //code by Joey Bertrand
    //
    GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Block);
    GetMesh()->SetGenerateOverlapEvents(true);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    
    //code by Joey Bertrand
    //
    Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attribute"));
    HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health"));
    HealthBarWidget->SetupAttachment(GetRootComponent());
    
    //code by Joey Bertrand
    //
    GetCharacterMovement()->bOrientRotationToMovement = true;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationPitch = false;
    
    //code by Joey Bertrand.
    //
    PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
    PawnSensing->SightRadius = 3000.f;
    PawnSensing->SetPeripheralVisionAngle(65.f);
    
    // constructor used to initialize the damage component, wavemanager, and fix collision errors with the camera handling. Tristan hughes
    damageComponent = CreateDefaultSubobject<UdamageComponent>(TEXT("damage component initialized"));
    waveManager = Cast<AwaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AwaveManager::StaticClass()));
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}
//code by Joey Bertrand
//
void AenemyPatrolCharacter::PatrolTimerFinished()
{
    MoveToTarget(PatrolTarget);
}

// Called when the game starts or when spawned. written by unreal
void AenemyPatrolCharacter::BeginPlay()
{
    Super::BeginPlay();
    //patrol target with AI controller, code by Joey Bertrand
    enemyPatrolCharacterController = Cast<AAIController>(GetController());
    
    MoveToTarget(PatrolTarget);
    //code by Joey Bertrand
    if (HealthBarWidget)
    {
        HealthBarWidget->SetVisibility(false);
    }
    
    //patrol target with AI controller, cdoe by Joey Bertrand
    enemyPatrolCharacterController = Cast<AAIController>(GetController());
    MoveToTarget(PatrolTarget);
    
    if (PawnSensing)
    {
        
        PawnSensing->OnSeePawn.AddDynamic(this, &AenemyPatrolCharacter::PawnSeen);
        //FAIMoveRequest MoveRequest;
        //MoveRequest.SetGoalActor(PatrolTarget);
        //MoveRequest.SetAcceptanceRadius(20.f);
        //FNavPathSharedPtr TargetPath;
        //enemyPatrolCharacterController->MoveTo(MoveRequest, &TargetPath);
        //TArray<FNavPathPoint>& Pathpoints = TargetPath->GetPathPoints();
        //for (random& point : PathPoints)
    
            //const FVector& Location = Point.Location;
            //DrawDebugSphere(GetWorld(), Location, 20.f, 20, FColor::Green, false, 10.f);
            //}
        }
    }
    
void AenemyPatrolCharacter::Die()
    {
        //adding a play death animation
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance && DeathMontage)
        {
            AnimInstance->Montage_Play(DeathMontage);
            
            //Selecting randomm Montage
            const int32 Selection = FMath::RandRange(0, 5);
            FName SectionName = FName();
            
            
            switch (Selection)
            {
            case 0:
                SectionName = FName("Death1");
                DeathPose = EDeathPose::EDP_Death1;
                
                //case 1:
                //SectionName = FName("Death2");
                //DeathPose = EDeathPose::EDP_Death2;
                
                //case 2:
                //SectionName = Fname("Death3");
                //DeathPose = EDeathPose::EDP_Death3;
            default:
                break;
            }
            
            AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
        }
        if (HealthBarWidget)
        {
            HealthBarWidget->SetVisibility(false);
        }
        
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        SetLifeSpan(5.f);
    }
void AenemyPatrolCharacter::MoveToTarget(AActor* Target)
    {
        if (enemyPatrolCharacterController == nullptr || Target == nullptr) return;
        FAIMoveRequest MoveRequest;
        MoveRequest.SetGoalActor(Target);
        MoveRequest.SetAcceptanceRadius(20.f);
        enemyPatrolCharacterController->MoveTo(MoveRequest);
        
    }

//Enemy character behavior, sensing if target is in range and tracks its location. code by Joey Bertrand
bool AenemyPatrolCharacter::InTargetRange(AActor* Target, double Radius)
    {
        const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

        DrawDebugSphere(GetWorld(), GetActorLocation(), 20.0f, 12, FColor::Red, false, 1.0f);
        DrawDebugSphere(GetWorld(), Target->GetActorLocation(), 20.0f, 12, FColor::Blue, false, 1.0f);

        return DistanceToTarget <= Radius;
    }

AActor* AenemyPatrolCharacter::ChoosePatrolTarget()
    {
        TArray<AActor*> ValidTargets;
        for (AActor* Target : PatrolTargets)
        {
            if (Target != PatrolTarget)
            {
                ValidTargets.AddUnique(Target);
            }
        }

            const int32 NumberPatrol = PatrolTargets.Num();
            if (NumberPatrol > 0)
            {
                    const int32 SelectTarget = FMath::RandRange(0, PatrolTargets.Num() - 1);
                    AActor* target = PatrolTargets[TargetSelection];
                    PatrolTarget = target;
                    return PatrolTargets[TargetSelection];
            }
                return nullptr;
        }
        
void AenemyPatrolCharacter::PlayHitReactMontage(const FName& SectionName)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance && HitReactMontage)
        {
                AnimInstance->Montage_Play(HitReactMontage);
                AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
            }
        }
    
void AenemyPatrolCharacter::PawnSeen(APawn* SeenPawn)
    {
        if (enemystate == Eenemystate::ES_Chasing) return;
    
        if (SeenPawn->ActorHasTag(FName("developmentCharacterTH")))
        {
            GetWorldTimerManager().ClearTimer(PatrolTimer);
            GetCharacterMovement()->MaxWalkSpeed = 300.f;
            CombatTarget = SeenPawn;
            
            if (enemystate != Eenemystate::ES_Attacking)
            {
                enemystate = Eenemystate::ES_Chasing;
                MoveToTarget(CombatTarget);
            }
        }
    }
// Called every frame
void AenemyPatrolCharacter::Tick(float DeltaTime)
    {
    Super::Tick(DeltaTime);
    
//code by Joey Bertrand
    if (PatrolTarget)
    {
        if (InTargetRange(PatrolTarget, CombatRadius))
        {
            const int32 NumberPatrol = PatrolTargets.Num();
            if (NumberPatrol > 0)
            {
                const int32 SelectTarget = FMath::RandRange(0, NumberPatrol - 1);
                AActor* target = PatrolTargets[TargetSelection];
                PatrolTarget = target;
            }
           
        }
    }
    if (enemystate >Eenemystate::ES_PatrolTarget)
    {
       CheckCombatTarget();
        
    }
    else
    {
        CheckPatrolTarget();
    }
        
}


 //Called to bind functionality to input
void AenemyPatrolCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}
//Enemy character behavior, sensing if target is in range and tracks its location. code by Joey Bertrand


//code by Joey Bertrand
//checks to see if combat target is in radius range
void AenemyPatrolCharacter::CheckCombatTarget()
{
    if (!InTargetRange(CombatTarget, CombatRadius))
    {
        CombatTarget = nullptr;
        if (HealthBarWidget)
        {
            HealthBarWidget->SetVisibility(false);
        }
    }
}
//code by tristan hughes

// this function is designed to handle the functionality for the enemy taking damage
// if the function receives damage info then apply damage using the modular damage system
// it also stores the attacking actor                            //code by Joey Bertrand----------------------------------------->
    
void AenemyPatrolCharacter::takeDamage(const UdamageInfo* damageInfo, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
 {
    if (damageInfo) {
        if (damageComponent) {
            damageComponent->applyDamage(damageInfo);
            lastAttacker = damageInfo->attackingActor;
        }
    //code By joey bertrand, adding a visible health bar to enemy health after it takes damage from main character
    if(Attributes && HealthBarWidget)
    {
        Attributes->ReceiveDamage(damageInfo->damageAmount);
        HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
        
    }
        //enemy behavior, AI character reaction to take damage, Code by Joey Bertrand
        CombatTarget = EventInstigator->GetPawn();
        enemystate = Eenemystate::ES_Chasing;
        GetCharacterMovement()->MaxWalkSpeed = 200.f;
        MoveToTarget(CombatTarget);
    /////////////////////////////////////////////////////////////////////////////////////// Code by joey bertand above
    }
}

// this function is designed to do damage to an actor
// if an actor is detected than setup damage info for the attack
// if the cast to the development character is successful it will do damage to the main player, Tristan Hughes
void AenemyPatrolCharacter::doDamage(AActor* target)
  
    {
        if (target)
        {
            // Create a new damage info object
            UdamageInfo* damageInfo = NewObject<UdamageInfo>();
            //damageInfo->damageAmount = 20.0f; // Example: Set damage amount
            damageInfo->damageType = EDamageType::LightAttack;
            damageInfo->damageResponse = EDamageResponse::Melee;

            // Apply damage to the target
            UdamageComponent* TargetDamageComponent = target->FindComponentByClass<UdamageComponent>();
            if (TargetDamageComponent)
            {
                TargetDamageComponent->applyDamage(damageInfo);
            }
        }
    }

     if (target){
        UdamageInfo* damageInfo = NewObject<UdamageInfo>();

        damageInfo->damageAmount = 10.0;
        damageInfo->damageType = EDamageType::LightAttack;
        damageInfo->damageResponse = EDamageResponse::Melee;
        damageInfo->isIndestructible = false;

        AdevelopmentCharacter* mainPlayer = Cast<AdevelopmentCharacter>(target);
        if (mainPlayer) {
            mainPlayer->takeDamage(damageInfo);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }
    }


// this function was create as part of the wavemanager system
// it is designed in a way that if the actor we are targeting casts successfully and if the waveManager object is initialized correctly
// then tell the wave manager that the actor died
// destroy after 10 seconds, Tristan Hughes
void AenemyPatrolCharacter::destroy() {
    AdevelopmentCharacter* player = Cast<AdevelopmentCharacter>(lastAttacker);
    if (player && waveManager) {
        waveManager->enemyDeath();
    }
    SetLifeSpan(10.0);
}

*/
