
// damage info class for storing damage variables
// Developer(s): Tristan Hughes 
// Last Updated: 11-26-24

/*
Look into copyright notice ? this comment was included when creating the class: Fill out your copyright
notice in the Description page of Project Settings.
*/

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
	LightAttack UMETA(DisplayName = "Light Damage"),
	HeavyAttack UMETA(DisplayName = "Heavy Damage"),
};


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
