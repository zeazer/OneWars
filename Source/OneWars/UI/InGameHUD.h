// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuBase.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class ONEWARS_API UInGameHUD : public UMenuBase
{
	GENERATED_BODY()
public:
	virtual void Init() override;
};
