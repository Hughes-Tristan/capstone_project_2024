// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "damageComponent.h"
#include "damageInterface.h"
#include "damageInfo.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossCharacter.generated.h"

UCLASS()
class CAPSTONE_PROJECT_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossCharacter();
    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    
    //Damage System Boss Character interface
    virtual void applyHealth(float healthAmount);
    virtual void applyDamage(const UdamageInfo* damageInfo);
    virtual float getHealth() const;
    virtual float getMaxHealth() const;
    
    

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    void HandleHealth();
    
private:
    UPROPERTY(EditAnywhere, Category = "Boss Damage")
    UdamageComponent* damageComponent;
    

};
