
// Enemy character class for enemy behavior
// Developer(s): Tristan Hughes, Joey Bertrand
// Last Updated: 11-26-24

#include "enemycharacter1.h"
#include "developmentCharacterTH.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "waveManager.h"
#include "AIController.h"

// constructor used to initialize the damage component, wavemanager, and fix collision errors with the camera handling
Aenemycharacter1::Aenemycharacter1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    damageComponent = CreateDefaultSubobject<UdamageComponent>(TEXT("damage component initialized"));
    waveManager = Cast<AwaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AwaveManager::StaticClass()));
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
    GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

// Called when the game starts or when spawned. written by unreal
void Aenemycharacter1::BeginPlay()
{
	Super::BeginPlay();
	//if (HealthBarWidget)
   // {
       // HealthBarWidget->SetVisibility(false);
   // }
    
    //patrol target with AI controller by Joey Bertrand
   // enemycharacter1Controller = Cast<AIController>(GetController());
    //if (enemycharacter1Controller)
    //{
        //FAIMoveRequest MoveRequest;
        //MoveRequest.SetGoalActor(PatrolTarget);
        //MoveRequest.SetAcceptanceRadius(15.f);
        //FNavPathSharedPtr TargetPath;
        //enemycharacter1Controller->MoveTo(MoveRequest, &TargetPath);
        //TArray<FNavPathPoint>& Pathpoints = TargetPath->GetPathPoints();
        //for (random& point : PathPoints)
        //{
            //const FVector& Location = Point.Location;
            //DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Green, false, 10.f);
        //}
    //}
}
// Called every frame
void Aenemycharacter1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
//code by Joey Bertrand

   // if (Aenemycharacter1 >Eenemystate::ES_PatrolTarget)
    //{
      //  CheckCombatTarget();
        
    //}
    //else
    //{
      //  CheckPatrolTarget();
    //}
        
}

//void Aenemycharacter1::CheckPatrolTarget()



 //Called to bind functionality to input
void Aenemycharacter1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
//Enemy character behavior, sensing if target is in range and tracks its location. code by Joey Bertrand
//bool Aenemycharacter1::InTargetRange(AActor* Target, double Radius)
//{
  //  const double DistanceToTarget = (Target->GetActorLocation())-GetActorLocation()).size();
    
    //Draw_Sphere_SingleFrame(GetActorLocation());
    //Draw_Sphere_SingleFrame(Target->GetActorLocation());
    
    //return DistanceToTarget <= Radius;
//}

//code by Joey Bertrand
//checks to see if combat target is in radius range
/*void Aenemycharacter1::CheckCombatTarget()
{
    if (!TargetinRange(CombatTarget, CombatRadius))
    {
        CombatTarget = nullptr;
        if (HealthBarWidget)
        {
            HealthBarWidget->SetVisibility(false);
        }
    }
}

if (PatrolTarget)
{
    if (TargetinRange(PatrolTarget, CombatRadius))
    {
        const int32 NumberPatrol = PatrolTargets.Num();
        if (NumberPatrol > 0)
        {
            const int32 SelectTarget = FMath::RandRange(0, NumberPatrol - 1);
            AActor* Target = PatrolTargets[TargetSelection];
            PatrolTarget = Target;
        }
       
    }
}
*/
//code by tristan hughes

// this function is designed to handle the functionality for the enemy taking damage
// if the function receives damage info then apply damage using the modular damage system
// it also stores the attacking actor
void Aenemycharacter1::takeDamage(const UdamageInfo* damageInfo) {
    if (damageInfo) {
        if (damageComponent) {
            damageComponent->applyDamage(damageInfo);
            lastAttacker = damageInfo->attackingActor;
        }
    }
}

// this function is designed to do damage to an actor
// if an actor is detected than setup damage info for the attack
// if the cast to the development character is successful it will do damage to the main player 
void Aenemycharacter1::doDamage(AActor* target) {
    if (target) {
        UdamageInfo* damageInfo = NewObject<UdamageInfo>();

        //damageInfo->damageAmount = 10.0;
        damageInfo->damageType = EDamageType::LightAttack;
        damageInfo->damageResponse = EDamageResponse::Melee;
        damageInfo->isIndestructible = false;

        AdevelopmentCharacter* mainPlayer = Cast<AdevelopmentCharacter>(target);
        if (mainPlayer) {
            mainPlayer->takeDamage(damageInfo);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }
    }
}


// this function was create as part of the wavemanager system
// it is designed in a way that if the actor we are targeting casts successfully and if the waveManager object is initialized correctly
// then tell the wave manager that the actor died
// destroy after 10 seconds
void Aenemycharacter1::destroy() {
    AdevelopmentCharacter* player = Cast<AdevelopmentCharacter>(lastAttacker);
    if (player && waveManager) {
        waveManager->enemyDeath();
    }
    SetLifeSpan(10.0);
}
