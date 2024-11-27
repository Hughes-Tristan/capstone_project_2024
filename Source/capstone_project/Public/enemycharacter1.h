// Fill out your copyright notice in the Description page of Project Settings.


#pragma once



#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "damagecomponent.h"
//#include "enemycharacter1states.generated.h"
#include "enemycharacter1.generated.h"


class UanimationMontage;
class UAttributeComponent;
class Upawnsensingcomponent;


UCLASS()
class CAPSTONE_PROJECT_API Aenemycharacter1 : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	Aenemycharacter1();
    
    
    //tristan hughes
    UFUNCTION(BlueprintCallable, Category = "Damage")
    void takeDamage(const UdamageInfo* damageInfo);

    UFUNCTION(BlueprintCallable, Category = "Damage")
    void doDamage(AActor* target);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
//code written by joey bertrand
    UFUNCTION (BlueprintCallable, Category = "combat")
    void CheckCombatTarget();
    */
    
    
    
    
    


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    /*
    //code written by joey bertrand
    UFUNCTION (BlueprintCallable, Category = "Patrol")
    void CheckPatrolTarget();
    */
    
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    


    
private:
    
    UdamageComponent* damageComponent;
    /*
    UPROPERTY()
    class AAIController* enemycharacter1Controller;
    
    // AI navigation by Joey Bertrand
    UPROPERTY(EditOnly, Category = " AI navigation")
    AActor* PatrolTarget;
    
    UPROPERTY(EditOnly, Category = " AI navigation")
    TArray<AActor*> PatrolTargets;
    
    UPROPERTY(EDIT)
    double PatrolRadius = 230.f;
    */
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    /*
    void die();
    bool InTargetRange(AActor* Target, double Radius)
    {
        
    }
  */
    
};

