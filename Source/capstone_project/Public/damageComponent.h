
// Damage Component Class for modular damage system
// Developer(s): Tristan Hughes 
// Last Updated: 11-26-24

/*
Look into copyright notice ? this comment was included when creating the class: Fill out your copyright
notice in the Description page of Project Settings.
*/
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "damageInterface.h"
#include "damageInfo.h"


#include "damageComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CAPSTONE_PROJECT_API UdamageComponent : public UActorComponent, public IdamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UdamageComponent();

	// virtual functions from the damage interface
	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual void applyHealth(float healthAmount) override;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual void applyDamage(const UdamageInfo* damageInfo) override;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual float getHealth() const override;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual float getMaxHealth() const override;

	// handles death physics
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void death();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	float health;
	const float maxHealth = 100;
	bool isDead;
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
