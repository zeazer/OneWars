// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "Components/SceneComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "OWHexTile.generated.h"

UCLASS()
class ONEWARS_API UOWHexTile : public UHierarchicalInstancedStaticMeshComponent
{
	GENERATED_BODY()
	
public:	
	UOWHexTile();


#pragma region
	const FVector& GetSize();

	const FRotator& GetRotation();
#pragma endregion

	void SetMaterialToDefault();
private:
	UMaterialInstanceDynamic* CreateMaterialInstance(const FColor& color, float opacity);

	UPROPERTY()
	UMaterialInterface* mMasterMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* mDefaultMaterial;

	FVector mSize;
	FRotator mRotation;
};
