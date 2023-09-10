// Copyright Epic Games, Inc. All Rights Reserved.

#include "RandomFPSGameMode.h"
#include "RandomFPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARandomFPSGameMode::ARandomFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
