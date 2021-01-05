// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OWHexGrid.generated.h"
class UInstancedStaticMeshComponent;
class UProceduralMeshComponent;

UCLASS()
class ONEWARS_API AOWHexGrid : public AActor
{
	GENERATED_BODY()

public:
	AOWHexGrid();

	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	FVector GetTilePosition(int32 tileIndex);

	void SelectTile(int32 index);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInstancedStaticMeshComponent* mInstancedHexTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* mSelectionMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 mRows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 mColumns;

	FVector mSize;
	FRotator mRotation;

private:
	void CreateGrid();

	int32 mCurrentTileIndex;
};
