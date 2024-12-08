// Fill out your copyright notice in the Description page of Project Settings.


#include "enemycharacter1.h"
#include "developmentCharacterTH.h"
#include "Components/PrimitiveComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "waveManager.h"
#include "AIController.h"
//#include "enemycharacter1states.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CoreMinimal.h"

// constructor used to initialize the damage component, wavemanager, and fix collision errors with the camera handling
// also used to initialize components for enemy attacking
Aenemycharacter1::Aenemycharacter1()
{
     // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    damageComponent = CreateDefaultSubobject<UdamageComponent>(TEXT("damage component initialized"));
    waveManager = Cast<AwaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AwaveManager::StaticClass()));
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
    GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
    
    // enemy attack initializations
    sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("attackSphere"));
    sphereComponent->SetCollisionProfileName(TEXT("collisionOverlap"));

    sphereComponent->SetSphereRadius(100.0f);
    sphereComponent->SetGenerateOverlapEvents(true);
    sphereComponent->SetupAttachment(RootComponent);
    sphereComponent->OnComponentBeginOverlap.AddDynamic(this, &Aenemycharacter1::OnAttackRangeOverlapBegin);

    canAttack = true;
    cooldownTime = 1.0f;
    EnemyState = EEnemyState::EES_Attacking;
}

// Called when the game starts or when spawned
void Aenemycharacter1::BeginPlay()
{
	Super::BeginPlay();
    waveManager = Cast<AwaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AwaveManager::StaticClass()));
}

// Called every frame
void Aenemycharacter1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    DrawDebugSphere(GetWorld(), sphereComponent->GetComponentLocation(), sphereComponent->GetScaledSphereRadius(), 12, FColor::Red, false, -1.0f, 0, 2.0f);

}

// Called to bind functionality to input
void Aenemycharacter1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// this function is designed to handle the functionality for the enemy taking damage
// if the function receives damage info then apply damage using the modular damage system
// it also stores the attacking actor
void Aenemycharacter1::takeDamage(const UdamageInfo* damageInfo) {
    if (damageInfo) {
        if (damageComponent) {

            damageComponent->applyDamage(damageInfo);
            lastAttacker = damageInfo->attackingActor;
            if (damageComponent->isDead) {
                destroy();
            }
        }
    }
}

void Aenemycharacter1::OnAttackRangeOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    AdevelopmentCharacter* mainCharacter;
    if (OtherActor && OtherActor != this && canAttack) {

        mainCharacter = Cast<AdevelopmentCharacter>(OtherActor);
        if (mainCharacter)
        {
            //Joey Bertrand
            PlayAttackMontage();
            
            doDamage(mainCharacter);
            canAttack = false;
            GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &Aenemycharacter1::shouldAttack, cooldownTime, false);
        }
    }
}

// this function is designed to do damage to an actor
// if an actor is detected than setup damage info for the attack
// if the cast to the development character is successful it will do damage to the main player
void Aenemycharacter1::doDamage(AActor* target) {
    if (target) {
        UdamageInfo* damageInfo = NewObject<UdamageInfo>();

        //Joey Bertrand
        PlayAttackMontage();
        
        //damageInfo->damageAmount = 10.0;
        damageInfo->damageType = EDamageType::EnemyAttack;
        damageInfo->damageResponse = EDamageResponse::Melee;
        damageInfo->isIndestructible = false;
        
        AdevelopmentCharacter* mainPlayer = Cast<AdevelopmentCharacter>(target);
        if (mainPlayer) {
            mainPlayer->takeDamage(damageInfo);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }
    }
}

void Aenemycharacter1::PlayAttackMontage()
{
    
    UAnimInstance* && AnimInstance = GetMesh()->GetAnimInstance();
    
    if (AnimInstance && AttackMontage)
    {
        AnimInstance->Montage_Play(AttackMontage);
        const int32 RandomSelection = FMath::RandRange(0, 2);
        
        FName SectionName = FName();
        switch(RandomSelection)
        {
            case 0:
                        SectionName = FName("Attack 1");
                break;
                
            case 1:
                        SectionName = FName("Attack 2");
                break;
                
            case 2:
                        SectionName = FName("Attack 3");
                
            default:
                    break;
                
                
        }
        AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
    }
}

void Aenemycharacter1::CheckCombatTarget()
{
    if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking)
    {
        EnemyState = EEnemyState::EES_Attacking;
        //Animation Attack Montage called
        PlayAttackMontage();
        doDamage(CombatTarget);
    }
}

bool Aenemycharacter1::InTargetRange(AActor* Target, double Radius)
{
    if(!Target) return false;
    const double Distance = (Target->GetActorLocation() - GetActorLocation()).Size();

    return Distance <= Radius;
    
}
//code by tristan hughes
// this function was create as part of the wavemanager system
// it is designed in a way that if the actor we are targeting casts successfully and if the waveManager object is initialized correctly
// then tell the wave manager that the actor died
// destroy after 10 seconds
void Aenemycharacter1::destroy() {
    AdevelopmentCharacter* player = Cast<AdevelopmentCharacter>(lastAttacker);
    if (player && waveManager) {
        if (waveManager->IsValidLowLevel()) {
            waveManager->enemyDeath();
        }
    }
    SetLifeSpan(10.0);
}

void Aenemycharacter1::shouldAttack() {
    canAttack = true;
}


