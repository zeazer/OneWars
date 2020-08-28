// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OWHexGrid.generated.h"
class UOWHexTile;

UCLASS()
class ONEWARS_API AOWHexGrid : public AActor
{
	GENERATED_BODY()

public:
	AOWHexGrid();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 mRows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 mColumns;
private:
	void CreateGrid();

};
