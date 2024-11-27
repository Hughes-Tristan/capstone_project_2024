
// Damage Interface Class for Modular Damage System
// Developer(s): Tristan Hughes 
// Last Updated: 11-26-24
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

