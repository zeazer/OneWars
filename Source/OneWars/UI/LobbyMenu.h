// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuBase.h"
#include "LobbyMenu.generated.h"

/**
 * 
 */
UCLASS()
class ONEWARS_API ULobbyMenu : public UMenuBase
{
	GENERATED_BODY()
public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void RefreshGames();
};
