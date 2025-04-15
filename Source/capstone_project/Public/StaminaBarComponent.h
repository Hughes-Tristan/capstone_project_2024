
/*******************************************************************************************
*
*   Stamina Bar Component v1.0.0 - Stamina Bar Component Class for Stamina Bar Class
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
#include "Components/WidgetComponent.h"
#include "StaminaBarComponent.generated.h"

/**
 * 
 */
// this is the stamina component class used for handling visual representation
UCLASS()
class CAPSTONE_PROJECT_API UStaminaBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public: 
	// constructor
	UStaminaBarComponent();
	
	// public setter method
	void setStaminaAmount(float percent);

protected:
	// protected reference to the stamina bar widget
	UPROPERTY()
	class UStaminaBar* staminaBarWidget;
	
};
