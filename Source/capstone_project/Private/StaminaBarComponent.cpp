
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

#include "StaminaBarComponent.h"
#include "StaminaBar.h"
#include "Components/ProgressBar.h"

// this is the constructor for the stamina bar component
//initializes the stamina bar widget to a null pointer
UStaminaBarComponent::UStaminaBarComponent() {
	staminaBarWidget = nullptr;
}

// this is a setter method for setting the stamina amount 
// it is used to update the visual representation of the stamina bar
// it tries to cast the widget object to the stamina bar if there is nothing initialized
// if the cast is successful then it will update the stamina percent based on the provided parameter
void UStaminaBarComponent::setStaminaAmount(float percent) {
	if (staminaBarWidget == nullptr) {
		staminaBarWidget = Cast<UStaminaBar>(GetUserWidgetObject());
	}
	if (staminaBarWidget && staminaBarWidget->staminaBar) {
		staminaBarWidget->staminaBar->SetPercent(percent);
	}
}
