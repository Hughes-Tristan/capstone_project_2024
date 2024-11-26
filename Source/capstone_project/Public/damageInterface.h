// Fill out your copyright notice in the Description page of Project Settings.

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

class CAPSTONE_PROJECT_API IdamageInterface
{
	GENERATED_BODY()

public:
	virtual void applyHealth(float healthAmount) = 0;
	virtual void applyDamage(const UdamageInfo* damageInfo) = 0;
	virtual float getHealth() const = 0;
	virtual float getMaxHealth() const = 0;
};

