// Fill out your copyright notice in the Description page of Project Settings.


#include "developmentGamemodeTH.h"
#include "developmentCharacterTH.h"
#include "UObject/ConstructorHelpers.h"

AdevelopmentGamemode::AdevelopmentGamemode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Content/Characters/Development/BP_developmentCharacter_TJ"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}