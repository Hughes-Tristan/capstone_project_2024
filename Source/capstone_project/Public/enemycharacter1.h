// Fill out your copyright notice in the Description page of Project Settings.
// Enemy character class for enemy behavior
// Developer(s): Tristan Hughes, Joey Bertrand
// Last Updated: 11-26-24

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
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    float MinSpeed = 200.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    float MaxSpeed= 350.f;
    
    
    // Code written by Tristan Hughes
    // these are functions and objects used in the damage system and wave manager
    UFUNCTION(BlueprintCallable, Category = "Damage")
    void takeDamage(const UdamageInfo* damageInfo);

    UFUNCTION(BlueprintCallable, Category = "Damage")
    void doDamage(AActor* target);

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
