// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OWGrid.generated.h"
class UProceduralMeshComponent;

UCLASS()
class ONEWARS_API AOWGrid : public AActor
{
	GENERATED_BODY()

public:
	AOWGrid();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void LocationToTile(const FVector& worldLocation, bool& isValid, int32& row, int32& column);

	void TileToGridLocation(int32 row, int32 column, bool isCenter, bool& isValid, FVector2D& gridLcoation);

	void SetSelectedTile(int32 row, int32 column);

protected:
	void CreateVerticalLines();

	void CreateHorizontalLines();

	void CreateHexagonGrid();

	void CreateLine(const FVector& startPosition, const FVector& endPosition, float thickness, TArray<FVector>& vertices, TArray<int32>& triangles);

	void CreateForm(const FVector& centerPosition, int32 size, float thickness, TArray<FVector>& vertices, TArray<int32>& triangles);

	void CreateFilledHex(const FVector& centerPosition, int32 size, float thickness, TArray<FVector>& vertices, TArray<int32>& triangles);

	float GetGridWitdh() const;

	float GetGridHeight() const;

	UMaterialInstanceDynamic* CreateMaterialInstance(const FColor& color, float opacity);

	void UpdateMesh();

	void CreateGridMesh();

	void CreateSelectionMesh();

	bool IsTileValid(int32 row, int32 column);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* mGridMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* mSelectionMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "NumberOfRows"))
	int32 mNumberOfRows;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "NumberOfColumns"))
	int32 mNumberOfColumns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "TileSize"))
	float mTileSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "LineThickness"))
	float mLineThickness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "LineColor"))
	FColor mLineColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "SelectionColor"))
	FColor mSelectionColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "LineOpacity"))
	float mLineOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "SelectionOpacity"))
	float mSelectionOpacity;
private:
	UMaterialInterface* mMasterMaterial;

	UMaterialInstanceDynamic* mLineMaterial;

	UMaterialInstanceDynamic* mSelectedMaterial;

	TArray<FVector> mGridVertices;
	TArray<int32> mGridTriangles;

	TArray<FVector> mSelectionVertices;
	TArray<int32> mSelectionTriangles;
};
