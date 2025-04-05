
/*******************************************************************************************
*
*   Stamina Component v1.0.0 - Stamina Component Class for Stamina System
*
*   Last Modified: 4-5-25
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
#include "StaminaComponent.generated.h"

// this is a class for representing the stamina system for an actor
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAPSTONE_PROJECT_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// constructor
	UStaminaComponent();

	// public methods associated with the stamina system
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void takeStamina(float amountTaken);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	bool isStaminaAvailable(float amountAvailable) const;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float getStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float getMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float getStaminaAmount() const;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void setStaminaGain(bool shouldGain);



protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// private variables associated with the stamina system
	UPROPERTY(EditAnywhere, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	float stamina;

	UPROPERTY(EditAnywhere, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	float maxStamina;

	UPROPERTY(EditAnywhere, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	float regenerationRate;

	UPROPERTY(EditAnywhere, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	float regenerationDelay;


	bool shouldRegenerate, isRegenerating;
	float timeSinceDrained;

};
