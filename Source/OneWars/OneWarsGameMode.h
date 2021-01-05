// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OneWarsGameMode.generated.h"
class AOWGrid;
class AOWHexGrid;

UCLASS(minimalapi)
class AOneWarsGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOneWarsGameMode();

	UFUNCTION(BlueprintCallable)
	AOWGrid* GetGrid();

	UFUNCTION(BlueprintCallable)
	void SetGrid(AOWGrid* grid);

	UFUNCTION(BlueprintCallable)
	AOWHexGrid* GetHexGrid();

	UFUNCTION(BlueprintCallable)
	void SetHexGrid(AOWHexGrid* grid);
private:
	AOWGrid* mGrid;
	AOWHexGrid* mHexGrid;
};



