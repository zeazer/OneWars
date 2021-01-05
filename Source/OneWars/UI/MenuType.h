// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MenuType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EMenuType
{
	Default     UMETA(DisplayName = "Default, this isn't an actuall menu"),
	MainMenu     UMETA(DisplayName = "Main Menu"),
	Settings      UMETA(DisplayName = "Settings"),
	CharacterSelect   UMETA(DisplayName = "Character Select"),
	InGameHUD	UMETA(DisplayName = "In Game HUD"),
};
