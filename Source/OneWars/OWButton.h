// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "OWButton.generated.h"
/**
 *
 */
UCLASS()
class ONEWARS_API UOWButton : public UButton
{
	GENERATED_BODY()

public:
	UOWButton();

	UFUNCTION( BlueprintCallable )
	const FString& PressedButton();


private:
	FString mPressedButtonText;
};
