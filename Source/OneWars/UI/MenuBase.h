// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuType.h"
#include "MenuBase.generated.h"
/**
 *
 */
UCLASS()
class ONEWARS_API UMenuBase : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void Init();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Intro();
	void virtual IntroInternal();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Outro();
	void virtual OutroInternal();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu Type")
	TEnumAsByte<EMenuType> mMenuType;

	class APlayerHUD* GetPlayerHUD();
};
