
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

#include "StaminaComponent.h"

// this is the class constructor for setting all default values
UStaminaComponent::UStaminaComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	maxStamina = 100.0f;
	stamina = maxStamina;
	regenerationRate = 10.0f;
	regenerationDelay = 1.0f;
	shouldRegenerate = true;
	isRegenerating = false;
	timeSinceDrained = 0.0f;
}

// this is called when game starts and sets the initial stamina value to the maximum stamina value
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	stamina = maxStamina;
}

// this method is called every frame and set to true in the constructor
// it disables stamina if the character shouldnt regenerate
// if the stamina is less that the maximum stamina then it will try to regenerate the stamina
void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!shouldRegenerate) {
		return;
	}


	if (stamina < maxStamina)
	{
		timeSinceDrained += DeltaTime;
		if (timeSinceDrained >= regenerationDelay)
		{
			float regenAmount = regenerationRate * DeltaTime;
			stamina = FMath::Min(stamina + regenAmount, maxStamina);
		}
	}
	
}

// this method reduces the stamina amount by the provided amount taken
// if none is taken then ignore otherwise reduce stamina but not less than 0 and reset time since last drain
void UStaminaComponent::takeStamina(float amountTaken) {
	if (amountTaken <= 0.0f) {
		return;
	}
	stamina = FMath::Max(stamina - amountTaken, 0.0f);
	timeSinceDrained = 0.0f;
}

// this method checks for if there is stamina available
bool UStaminaComponent::isStaminaAvailable(float amountAvailable) const {
	return stamina >= amountAvailable;
}

// this is a getter method for current stamina value
float UStaminaComponent::getStamina() const {
	return stamina;
}

// this is a getter method for maximum stamina value
float UStaminaComponent::getMaxStamina() const {
	return maxStamina;
}

// this is a getter method for current stamina amount as a percentage
float UStaminaComponent::getStaminaAmount() const {
	if (maxStamina > 0.0f)
	{
		return stamina / maxStamina;
	}
	else
	{
		return 0.0f;
	}
}

// this is a setter method for enabling and disabling stamina regen
void UStaminaComponent::setStaminaGain(bool shouldGain) {
	shouldRegenerate = shouldGain;
}

