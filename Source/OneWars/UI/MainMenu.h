// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuBase.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class ONEWARS_API UMainMenu : public UMenuBase
{
	GENERATED_BODY()
public:
	virtual void Init() override;
};
