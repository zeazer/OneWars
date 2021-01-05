// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuBase.h"
#include "../PlayerHud.h"

void UMenuBase::Intro_Implementation()
{
	IntroInternal();
}

void UMenuBase::Init()
{
	IntroInternal();
}

void UMenuBase::IntroInternal()
{
}

void UMenuBase::Outro_Implementation()
{
	OutroInternal();
}

void UMenuBase::OutroInternal()
{
}

APlayerHUD* UMenuBase::GetPlayerHUD()
{
	APlayerController* playercontroller = GetGameInstance()->GetFirstLocalPlayerController();
	return Cast<APlayerHUD>(playercontroller->GetHUD());
}
