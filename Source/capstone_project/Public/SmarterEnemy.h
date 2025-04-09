// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "damageComponent.h"
#include "damageInfo.h"
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
    float MinSpeed = 350.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MaxSpeed = 680.0f;

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
