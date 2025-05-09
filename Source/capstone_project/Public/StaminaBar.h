
/*******************************************************************************************
*
*   Stamina Bar v1.0.0 - Stamina Bar Class for Widget Blueprint
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
*   Copyright � 2025 Tristan Hughes and 2025 UNR Capstone Team 10. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is personal and confidential unless stated otherwise.
*   Permission for use in any form must be granted in writing by Tristan Hughes and the 2025 UNR Capstone Team 10.
*
**********************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StaminaBar.generated.h"

/**
 * 
 */
// this class is used to represent the stamina bar widget for the user interface
UCLASS()
class CAPSTONE_PROJECT_API UStaminaBar : public UUserWidget
{
	GENERATED_BODY()

public:
    // constructor
    UStaminaBar(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    // reference to the progress bar widget class 
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* staminaBar;
};
