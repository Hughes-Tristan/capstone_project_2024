
/*******************************************************************************************
*
*   Smarter Enemy v1.0.0 - Smarter Enemy Class for Enemy Behavior
*
*   Last Modified: 4-15-25
*
*   MODULE USAGE:
*	** Module usage section WIP **
*
*   DISCLAIMER: The "Module Usage" section of this header comment was generated with the assistance of generative AI.
*
*   LICENSE: Personal Use
*
*   Copyright � 2025 UNR Capstone Team 10. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is personal and confidential unless stated otherwise.
*   Permission for use in any form must be granted in writing by the 2025 UNR Capstone Team 10.
*
**********************************************************************************************/

#include "SmarterEnemy.h"
#include "developmentCharacterTH.h"
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

    audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    audioComponent->SetupAttachment(RootComponent);
    audioComponent->bAutoActivate = false;
    minPitch = 0.6f;
    maxPitch = 1.4f;

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

    TArray<UPrimitiveComponent*> componentsArray;
    GetComponents<UPrimitiveComponent>(componentsArray);
    for (int32 i = 0; i < componentsArray.Num(); ++i) {
        UPrimitiveComponent* component = componentsArray[i];
        if (component) {
            component->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
        }
    }
	
}

// Called every frame
void ASmarterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    UE_LOG(LogTemp, Warning, TEXT("enemy: %s, controller: %s, ai controller: %s"),
        *GetName(),
        GetController() ? *GetController()->GetName() : TEXT("none"),
        Cast<AAIController>(GetController()) ? TEXT("valid") : TEXT("invalid"));
}

// Called to bind functionality to input
void ASmarterEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// this function is designed to handle the functionality for the enemy taking damage
// if the function receives damage info then apply damage using the modular damage system
// it also stores the attacking actor
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

// this function is an event function provided by the Character class and is triggered when an actor enters the range
// it checks if the actor is valid, in range, and can attack
// then check if the main character exists then set the current taget to the main character
void ASmarterEnemy::OnAttackRangeOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    AdevelopmentCharacter* mainCharacter;
    if (OtherActor && OtherActor != this) {

        mainCharacter = Cast<AdevelopmentCharacter>(OtherActor);
        if (mainCharacter)
        {
            
            //doDamage(mainCharacter);
            //canAttack = false;
            //GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ASmarterEnemy::shouldAttack, cooldownTime, false);
            currentTarget = mainCharacter;
        }
    }
}

// this function is designed to do damage to an actor
// if an actor is detected than setup damage info for the attack
// if the cast to the development character is successful it will do damage to the main player
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
        }
    }
}

// this function was created as part of the wavemanager system
// it is designed in a way that if the actor we are targeting casts successfully and if the waveManager object is initialized correctly
// then tell the wave manager that the actor died
// destroy after 10 seconds
// if there is a blueprint to spawn then determine the objects spawning based in its percent
// then configure parametors for the actor and spawn the actor
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

// this function is a method to reset the canAttack flag
void ASmarterEnemy::shouldAttack() {
    canAttack = true;
}


// this function is called in blueprint when an animation notifier is triggered
// when triggeed it will check if there is a currentTarget and if the canAttack flag is true
// if it is true then it will do damgage to the target, it will player the attack hit sound, and it will cause the players camera to shake
// it then resets the canAttack flag on a timer
void ASmarterEnemy::onAttackHit() {
    UE_LOG(LogTemp, Warning, TEXT("OnAttackHit called on %s!"), *GetName());
    if (currentTarget && canAttack)
    {
        doDamage(currentTarget);
        if (attackHitSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,
                attackHitSound,
                currentTarget->GetActorLocation(),
                1.0f,
                FMath::RandRange(minPitch, maxPitch)
            );
        }
        if (attackCameraShake)
        {
            AdevelopmentCharacter* playerChararacter = Cast<AdevelopmentCharacter>(currentTarget);
            if (playerChararacter)
            {
                APlayerController* playerController = Cast<APlayerController>(playerChararacter->GetController());
                if (playerController)
                {
                    playerController->ClientStartCameraShake(attackCameraShake, 1.0f);
                }
            }
        }
        canAttack = false;
        GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ASmarterEnemy::shouldAttack, cooldownTime, false);
    }

}

// this function is used to trigger the attack sounds with a variable pitch range
void ASmarterEnemy::onAttackSound(){
    if (attackSound && audioComponent)
    {
        float randomPitch = FMath::RandRange(minPitch, maxPitch);

        audioComponent->SetSound(attackSound);
        audioComponent->SetPitchMultiplier(randomPitch);
        audioComponent->Play();
    }
}


