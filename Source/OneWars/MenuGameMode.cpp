// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameMode.h"
#include "OneWarsPlayerController.h"

AMenuGameMode::AMenuGameMode()
{
	PlayerControllerClass = AOneWarsPlayerController::StaticClass();
}