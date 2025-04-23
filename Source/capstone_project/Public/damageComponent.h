
/*******************************************************************************************
*
*   Damage Component v1.0.0 - Damage Component Class for Modular Damage System
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
*   Copyright © 2025 Tristan Hughes and 2025 UNR Capstone Team 10. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is personal and confidential unless stated otherwise.
*   Permission for use in any form must be granted in writing by Tristan Hughes and the 2025 UNR Capstone Team 10.
*
**********************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "damageInterface.h"
#include "damageInfo.h"


#include "damageComponent.generated.h"

class uAttributeComponent;

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
	virtual void applyDamage(const UdamageInfo* damageInfo, float damageDirect) override;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual float getHealth() const override;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual float getMaxHealth() const override;

	// handles death physics
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void death();

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void applyDirectDamage(float damageAmount);

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void setHealth(float newHealth);

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void setMaxHealth(float newMaxHealth);

	bool isDead;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	float health;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float maxHealth;
	//bool isDead;
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
