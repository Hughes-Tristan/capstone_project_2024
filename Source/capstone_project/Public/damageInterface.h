
/*******************************************************************************************
*
*   Damage Interface v1.0.0 - Damage Interface Class for Modular Damage System
*
*   Last Modified: 11-26-24
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
#include "UObject/Interface.h"
#include "damageInfo.h"
#include "damageInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UdamageInterface : public UInterface
{
	GENERATED_BODY()
};

// damage interface for managing the modular damage system
class CAPSTONE_PROJECT_API IdamageInterface
{
	GENERATED_BODY()

public:
	virtual void applyHealth(float healthAmount) = 0;
	virtual void applyDamage(const UdamageInfo* damageInfo) = 0;
	virtual float getHealth() const = 0;
	virtual float getMaxHealth() const = 0;
};

