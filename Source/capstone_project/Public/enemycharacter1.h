
// Enemy character class for enemy behavior
// Developer(s): Tristan Hughes, Joey Bertrand
// Last Updated: 11-26-24

#pragma once



#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "damagecomponent.h"
#include "damageInfo.h"
//#include "waveManager.h"
//#include "enemycharacter1states.generated.h"
#include "enemycharacter1.generated.h"


class UanimationMontage;
class UAttributeComponent;
class Upawnsensingcomponent;


class AwaveManager;


UCLASS()
class CAPSTONE_PROJECT_API Aenemycharacter1 : public ACharacter
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

	UPROPERTY(BlueprintReadOnly, Category = "damage")
	AActor* lastAttacker;

	UPROPERTY()
	AwaveManager* waveManager;

	void destroy();

    /*
    // code written by joey bertrand
    UFUNCTION (BlueprintCallable, Category = "combat")
    void CheckCombatTarget();
    */


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	// damage component object
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

