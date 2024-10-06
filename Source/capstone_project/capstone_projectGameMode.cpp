// Copyright Epic Games, Inc. All Rights Reserved.

#include "capstone_projectGameMode.h"
#include "capstone_projectCharacter.h"
#include "UObject/ConstructorHelpers.h"

Acapstone_projectGameMode::Acapstone_projectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
