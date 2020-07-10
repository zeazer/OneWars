// Copyright Epic Games, Inc. All Rights Reserved.

#include "OneWarsGameMode.h"
#include "OneWarsPlayerController.h"
#include "OneWarsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOneWarsGameMode::AOneWarsGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AOneWarsPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}