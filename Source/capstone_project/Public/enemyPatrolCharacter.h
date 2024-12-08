// Fill out your copyright notice in the Description page of Project Settings.
// Enemy character class for enemy behavior
// Developer(s): Tristan Hughes, Joey Bertrand
// Last Updated: 11-29-24

/*
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "developmentCharacterTH.h"
#include "damagecomponent.h"
#include "damageInfo.h"
#include "AIController.h"
#include "developmentCharacterTH.h"
#include "enemycharacter1states.h"
#include "enemyPatrolCharacter.generated.h"

class UanimationMontage;
class UAttributeComponent;
class UHealthBarComponent;
class UWidgetComponent;
class UPawnSensingComponent;
class AwaveManager;



UCLASS()
class CAPSTONE_PROJECT_API AenemyPatrolCharacter : public ACharacter
{
	GENERATED_BODY()
public:
    // Sets default values for this character's properties
    AenemyPatrolCharacter();
    
    //Joey Bertrand
    virtual void Tick(float DeltaTime) override;
    void CheckPatrolTarget();
    void CheckCombatTarget();
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    //virtual void GetHitreact(const FVector& ImpactPoint) override;
    void DirectionalHitReact(const FVector& ImpactPoint);
    virtual void takeDamage(const UdamageInfo* damageInfo, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
    
    // Code written by Tristan Hughes
    // these are functions and objects used in the damage system and wave manager
    
    virtual void takeDamage(const UdamageInfo* damageInfo);

   
    virtual void doDamage(AActor* target);
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
    EDeathPose DeathPose;

    UPROPERTY(BlueprintReadOnly, Category = "damage")
    AActor* lastAttacker;

    UPROPERTY()
    AwaveManager* waveManager;

    void destroy();

    
    // code written by joey bertrand
    //UFUNCTION (BlueprintCallable, Category = "combat")
    //void CheckCombatTarget();
    
    
    //code written by joey bertrand
   // UFUNCTION (BlueprintCallable, Category = "Patrol")
    //void CheckPatrolTarget();
    
    
private:
    
    //Enemy Patrol target joey bertrand
    FTimerHandle PatrolTimer;
    void PatrolTimerFinished();

    UPROPERTY(EditAnywhere, Category = "AI Navigation")
    float WaitMin = 5.f;

    UPROPERTY(EditAnywhere, Category = "AI Navigation")
    float WaitMax = 10.f;

    Eenemystate enemystate = Eenemystate::ES_PatrolTarget;
    
    
    //enemy components joey Bertrand
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UAttributeComponent* Attributes;
    

    UPROPERTY(VisibleAnywhere)
    UHealthBarComponent* HealthBarWidget;

    UPROPERTY(VisibleAnywhere)
    UPawnSensingComponent* PawnSensing;
    
    //enemy Montages joey Bertrand
    UPROPERTY(EditDefaultsOnly, Category = Montages)
    UAnimMontage* HitReactMontage;

    UPROPERTY(EditDefaultsOnly, Category = Montages)
    UAnimMontage* DeathMontage;

    UPROPERTY(EditAnywhere, Category = Sounds)
    USoundBase* HitSound;

    UPROPERTY(EditAnywhere, Category = VisualEffects)
    UParticleSystem* HitParticles;

    UPROPERTY()
    AActor* CombatTarget;

    UPROPERTY(EditAnywhere)
    double CombatRadius = 500.f;

    UPROPERTY(EditAnywhere)
    double AttackRadius = 150.f;
    
    
    // damage component object
    UdamageComponent* damageComponent;

    UPROPERTY()
    class AAIController* enemyPatrolCharacterController;
    
    // AI navigation by Joey Bertrand
    UPROPERTY(EditAnywhere, Category = " AI navigation")
    AActor* PatrolTarget;
    
    UPROPERTY(EditAnywhere, Category = " AI navigation")
    TArray<AActor*> PatrolTargets;
    
    UPROPERTY(EditAnywhere)
    double PatrolRadius = 230.f;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    
    void Die();
    bool InTargetRange(AActor* Target, double Radius);
    void MoveToTarget(AActor* Target);
    AActor* ChoosePatrolTarget();

    UFUNCTION()
    
    void PawnSeen(APawn* SeenPawn);

    void PlayHitReactMontage(const FName& SectionName);

};

*/
