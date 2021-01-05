// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "../OneWarsGameInstance.h"

void UMainMenu::Init()
{
	APlayerController* playercontroller = GetGameInstance()->GetPrimaryPlayerController();
	if (playercontroller)
	{
		FInputModeUIOnly inputModeData;
		inputModeData.SetWidgetToFocus(TakeWidget());
		inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		playercontroller->SetInputMode(inputModeData);
		playercontroller->bShowMouseCursor = true;
	}
	Super::Init();
}