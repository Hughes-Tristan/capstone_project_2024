// Fill out your copyright notice in the Description page of Project Settings.
// Enemy character class for enemy behavior
// Developer(s): Tristan Hughes, Joey Bertrand
// Last Updated: 11-26-24

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "damagecomponent.h"
#include "damageInfo.h"
#include "enemycharacter1.generated.h"


class AwaveManager;
class USphereComponent;


UCLASS()
class CAPSTONE_PROJECT_API Aenemycharacter1: public ACharacter
{
    GENERATED_BODY()


public:
    // Sets default values for this character's properties
    Aenemycharacter1();
    
    
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

};
