// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "damageComponent.h"
#include "damageInfo.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShakeBase.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Components/PrimitiveComponent.h"
#include "SmarterEnemy.generated.h"

class AwaveManager;
class USphereComponent;

UCLASS()
class CAPSTONE_PROJECT_API ASmarterEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASmarterEnemy();
    
    UFUNCTION(BlueprintCallable, Category = "Damage")
    void takeDamage(const UdamageInfo* damageInfo);

    UFUNCTION(BlueprintCallable, Category = "Damage")
    void doDamage(AActor* target);
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MinSpeed = 150.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MaxSpeed = 380.0f;

    void shouldAttack();

    // fucntion for automatically detecting collisions
    UFUNCTION()
    void OnAttackRangeOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY(BlueprintReadOnly, Category = "damage")
    AActor* lastAttacker;

    UPROPERTY()
    AwaveManager* waveManager;

    void destroy();

    bool canAttack;
    FTimerHandle timerHandle;
    float cooldownTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> spawnBP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float spawnPercent;

    // animation notify methods and variables
    UPROPERTY(BlueprintReadWrite, Category = "Animation")
    AActor* currentTarget;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void onAttackHit();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void onAttackSound();

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundCue* attackSound;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundCue* attackHitSound;

    UPROPERTY(EditAnywhere, Category = "Audio")
    UAudioComponent* audioComponent;

    UPROPERTY(EditAnywhere, Category = "Audio")
    float minPitch = 0.8f;

    UPROPERTY(EditAnywhere, Category = "Audio")
    float maxPitch = 1.2f;

    UPROPERTY(EditAnywhere, Category = "Effects")
    TSubclassOf<UCameraShakeBase> attackCameraShake;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
    USphereComponent* sphereComponent;

    // damage component object
    UdamageComponent* damageComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
