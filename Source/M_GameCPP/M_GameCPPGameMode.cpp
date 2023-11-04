// Copyright Epic Games, Inc. All Rights Reserved.

#include "M_GameCPPGameMode.h"
#include "M_GameCPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

AM_GameCPPGameMode::AM_GameCPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}