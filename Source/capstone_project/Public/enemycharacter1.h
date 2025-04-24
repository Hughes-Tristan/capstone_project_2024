
/*******************************************************************************************
*
*   Enemy Character v1.0.0 - Enemy Character Class for Enemy Behavior
*
*   Last Modified: 12-10-24
*
*   MODULE USAGE:
*	** Module usage section WIP **
*
*   DISCLAIMER: The "Module Usage" section of this header comment was generated with the assistance of generative AI.
*
*   LICENSE: Personal Use
*
*   Copyright © 2025 UNR Capstone Team 10. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is personal and confidential unless stated otherwise.
*   Permission for use in any form must be granted in writing by the 2025 UNR Capstone Team 10.
*
**********************************************************************************************/

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "damagecomponent.h"
#include "damageInfo.h"
#include "enemycharacter1states.h"
#include "enemycharacter1.generated.h"


class AwaveManager;
class USphereComponent;
class UAnimMontage;

// this class is designed to control the enemy behavior for our enemy character
UCLASS()
class CAPSTONE_PROJECT_API Aenemycharacter1: public ACharacter
{
    GENERATED_BODY()


public:
    // Sets default values for this character's properties
    Aenemycharacter1();
    
    void CheckPatrolTarget();
    void CheckCombatTarget();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Combat")
    AActor* CombatTarget;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Combat")
    EEnemyState EnemyState;

    //Joey Bertrand
    UPROPERTY(EditAnywhere)
    double CombatRadius = 500.f;
    //Joey Bertrand
    UPROPERTY(EditAnywhere)
    double AttackRadius = 150.f;
    
    // Code written by Tristan Hughes
    // these are functions and objects used in the damage system and wave manager
    UFUNCTION(BlueprintCallable, Category = "Damage")
    void takeDamage(const UdamageInfo* damageInfo, float damage);

    UFUNCTION(BlueprintCallable, Category = "Damage")
    void doDamage(AActor* target, float damageAmount);
    
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
    //Joey Bertrand
    virtual void PlayAttackMontage();
    //Joey Bertrand
    UPROPERTY(EditDefaultsOnly, Category = Montages)
    UAnimMontage* AttackMontage;
    //Joey Bertrand
    bool InTargetRange(AActor* Target, double Radius);
    void MoveToTarget(AActor* Target);
    

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    
    
    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    


    
private:

	// sphere component for tracing enemy damage box
	//UPROPERTY(BlueprintReadOnly, Category = "damageSphere")
	USphereComponent* sphereComponent;

	// damage component object
    UdamageComponent* damageComponent;
    
    //Joey Bertrand
    UPROPERTY(EditAnywhere, Category = "AI Navigation")
    float WaitMin = 5.f;
    //Joey Bertrand
    UPROPERTY(EditAnywhere, Category = "AI Navigation")
    float WaitMax = 10.f;

};
