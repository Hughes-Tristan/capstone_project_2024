// 
// Code by: Tristan Hughes
// Date: 10-12-24

/*
Look into copyright notice ? this comment was included when creating the class: Fill out your copyright
notice in the Description page of Project Settings.
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DamageInfo.generated.h"


// Enum class for damage responses
UENUM(BlueprintType)
enum class EDamageResponse : uint8
{
	Melee UMETA(DisplayName = "Melee"),
	Death UMETA(DisplayName = "Object Death"),
};


// Enum class for damage types
UENUM(BlueprintType)
enum class EDamageType : uint8
{
	None UMETA(DisplayName = "No Damage"),
	LightAttack UMETA(DisplayName = "Light Damage"),
	HeavyAttack UMETA(DisplayName = "Heavy Damage"),
};


UCLASS(BlueprintType)
class CAPSTONE_PROJECT_API UDamageInfo : public UObject
{
	GENERATED_BODY()

public:

	UDamageInfo()
		: damageAmount(0.0), isIndestructible(false), damageType(EDamageType::LightAttack), damageResponse(EDamageResponse::Melee) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float damageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isIndestructible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageType damageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageResponse damageResponse;

};
