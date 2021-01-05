// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Nameplate.generated.h"

/**
 * 
 */
UCLASS()
class ONEWARS_API UNameplate : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
	class AOneWarsCharacter* mCharacter;
};
