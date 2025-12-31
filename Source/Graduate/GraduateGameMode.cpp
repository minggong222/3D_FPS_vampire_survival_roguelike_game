// Copyright Epic Games, Inc. All Rights Reserved.

#include "GraduateGameMode.h"
#include "GraduateCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGraduateGameMode::AGraduateGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
