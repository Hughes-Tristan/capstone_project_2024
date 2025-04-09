// Fill out your copyright notice in the Description page of Project Settings.


#include "SmarterEnemy.h"
#include "developmentCharacterTH.h"
#include "Components/PrimitiveComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "waveManager.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CoreMinimal.h"


// Sets default values
ASmarterEnemy::ASmarterEnemy()
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
    sphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASmarterEnemy::OnAttackRangeOverlapBegin);

    canAttack = true;
    cooldownTime = 1.0f;

    spawnPercent = .04;


}

// Called when the game starts or when spawned
void ASmarterEnemy::BeginPlay()
{
	Super::BeginPlay();
    
    float RandomMoveSpeed = FMath::RandRange(MinSpeed, MaxSpeed);
    
    if(GetCharacterMovement()){
        GetCharacterMovement()->MaxWalkSpeed = RandomMoveSpeed;
    }
    waveManager = Cast<AwaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AwaveManager::StaticClass()));
	
}

// Called every frame
void ASmarterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASmarterEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASmarterEnemy::takeDamage(const UdamageInfo* damageInfo) {
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

void ASmarterEnemy::OnAttackRangeOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    AdevelopmentCharacter* mainCharacter;
    if (OtherActor && OtherActor != this && canAttack) {

        mainCharacter = Cast<AdevelopmentCharacter>(OtherActor);
        if (mainCharacter)
        {
            
            doDamage(mainCharacter);
            canAttack = false;
            GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ASmarterEnemy::shouldAttack, cooldownTime, false);
        }
    }
}
void ASmarterEnemy::doDamage(AActor* target) {
    if (target) {
        UdamageInfo* damageInfo = NewObject<UdamageInfo>();

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

void ASmarterEnemy::destroy() {
    AdevelopmentCharacter* player = Cast<AdevelopmentCharacter>(lastAttacker);
    if (player && waveManager) {
        if (waveManager->IsValidLowLevel()) {
            waveManager->enemyDeath();
        }
    }
    SetLifeSpan(10.0);
    if (spawnBP) {
        if (FMath::RandRange(0, 1) <= spawnPercent) {
            FActorSpawnParameters spawnParameters;
            spawnParameters.Owner = this;
            spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            GetWorld()->SpawnActor<AActor>(spawnBP, GetActorLocation(), FRotator::ZeroRotator, spawnParameters);
        }
    }
    
}


// used as a flag to reset the attack cooldown
void ASmarterEnemy::shouldAttack() {
    canAttack = true;
}


