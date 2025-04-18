
#include "developmentGamemodeTH.h"
#include "developmentCharacterTH.h"
#include "UObject/ConstructorHelpers.h"

AdevelopmentGamemode::AdevelopmentGamemode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<UClass> PlayerPawnBPClass(TEXT("/Game/Content/Characters/Development/BP_developmentCharacter_TJ.BP_developmentCharacter_TJ_C"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}