// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OWHexTile.generated.h"
class UProceduralMeshComponent;

UCLASS()
class ONEWARS_API UOWHexTile : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UOWHexTile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Root;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//UStaticMeshComponent* mTileMesh;

	void GenerateMesh();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "TileSize"))
	float mTileSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "LineColor"))
	FColor mTileColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "LineColor"))
	FColor mBorderColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "SelectionColor"))
	FColor mSelectionColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "LineOpacity"))
	float mTileOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "LineOpacity"))
	float mBorderOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "SelectionOpacity"))
	float mSelectionOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* mTileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* mBorderMesh;
#pragma region Get/Set
	const FVector& GetSize();
	UProceduralMeshComponent* GetTileMesh();
	UProceduralMeshComponent* GetBorderMesh();
	void SetRelativeLocation(const FVector& localLocation);
	void SetAsSelected(bool selected);
#pragma endregion

private:

	void CreateForm(const FVector& centerPosition, int32 size, TArray<FVector>& vertices, TArray<int32>& triangles);

	void CreateHex();

	UMaterialInstanceDynamic* CreateMaterialInstance(const FColor& color, float opacity);

	TArray<FVector> mTileVertices;
	TArray<int32> mTileTriangles;

	TArray<FVector> mBorderVertices;
	TArray<int32> mBorderTriangles;

	UPROPERTY()
	UMaterialInterface* mMasterMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* mDefaultMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* mBorderMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* mSelectedMaterial;

	FVector mSize;

};
