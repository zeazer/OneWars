// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHexTile.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "ProceduralMeshComponent.h"
#include "Math/UnrealMathVectorConstants.h"
#include "Engine/GameEngine.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

UOWHexTile::UOWHexTile()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> materialAsset(TEXT("Material'/Game/OneWars/Materials/GridMasterMaterial.GridMasterMaterial'"));
	if (materialAsset.Succeeded())
	{
		mMasterMaterial = materialAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> tileMesh(TEXT("StaticMesh'/Game/OneWars/Player/HexTile_mesh.HexTile_mesh'"));
	auto* mesh = tileMesh.Object;
	SetStaticMesh(mesh);

	mRotation = FRotator(0, 90, 0);
	mSize = mRotation.RotateVector(mesh->GetBoundingBox().GetSize());
	SetRelativeLocation(FVector::ZeroVector);
	bMultiBodyOverlap = true;
}

const FVector& UOWHexTile::GetSize()
{
	return mSize;
}

const FRotator& UOWHexTile::GetRotation()
{
	return mRotation;
}

void UOWHexTile::SetMaterialToDefault()
{
	if (!mDefaultMaterial)
	{
		mDefaultMaterial = CreateMaterialInstance(FColor::Black, 1.0f);
	}
	SetMaterial(0, mDefaultMaterial);
}

UMaterialInstanceDynamic* UOWHexTile::CreateMaterialInstance(const FColor& color, float opacity)
{
	auto* material = UMaterialInstanceDynamic::Create(mMasterMaterial, this);
	material->SetVectorParameterValue("Color", color);
	material->SetScalarParameterValue("Opacity", opacity);
	return material;
}