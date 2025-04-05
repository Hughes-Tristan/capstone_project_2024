
/*******************************************************************************************
*
*   Damage Info v1.0.0 - Damage Info Class for Storing Damage Variables
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
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "damageInfo.generated.h"

// Enum class for storing damage responses
UENUM(BlueprintType)
enum class EDamageResponse : uint8
{
	Melee UMETA(DisplayName = "Melee"),
	Death UMETA(DisplayName = "Object Death"),
};

// Enum class for storing damage types
UENUM(BlueprintType)
enum class EDamageType : uint8
{
	None UMETA(DisplayName = "No Damage"),
	EnemyAttack UMETA(DisplayName = "Enemy Damage"),
	LightAttack UMETA(DisplayName = "Light Damage"),
	HeavyAttack UMETA(DisplayName = "Heavy Damage"),
};

// class for storing damage information
UCLASS(BlueprintType)
class CAPSTONE_PROJECT_API UdamageInfo : public UObject
{
	GENERATED_BODY()

public:
	// contructor, variables, and objects for storing damage info used in the modular damage system
	UdamageInfo()
		: damageAmount(0.0), isIndestructible(false), attackingActor(nullptr), damageType(EDamageType::LightAttack), damageResponse(EDamageResponse::Melee) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float damageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isIndestructible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* attackingActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageType damageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageResponse damageResponse;
};
