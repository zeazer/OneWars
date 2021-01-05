// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class ONEWARS_API UServerInfoWidget : public UUserWidget
{
	GENERATED_BODY()
public:

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ServerInfo, meta = (AllowPrivateAccess = "true"))
	FName mServerName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ServerInfo, meta = (AllowPrivateAccess = "true"))
	int32 mNumberOfPlayers;
};
