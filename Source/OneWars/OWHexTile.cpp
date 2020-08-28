// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHexTile.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "ProceduralMeshComponent.h"
#include "Math/UnrealMathVectorConstants.h"
#include "Engine/GameEngine.h"

UOWHexTile::UOWHexTile()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	static ConstructorHelpers::FObjectFinder<UMaterial> materialAsset(TEXT("Material'/Game/OneWars/Materials/GridMasterMaterial.GridMasterMaterial'"));
	if (materialAsset.Succeeded())
	{
		mMasterMaterial = materialAsset.Object;
	}

	mTileMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TileMesh"));
	mTileMesh->SetupAttachment(Root);
	mTileMesh->SetVisibility(true);

	mBorderMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("BorderMesh"));
	mBorderMesh->SetupAttachment(Root);
	mBorderMesh->SetVisibility(true);

	mTileSize = 100;
	mBorderColor = FColor::Red;
	mTileColor = FColor::Blue;
	mSelectionColor = FColor::Green;
	mTileOpacity = 1.f;
	mBorderOpacity = 1.f;
	mSelectionOpacity = 1.f;

	//mTileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	//mTileMesh->SetupAttachment(Root);
	//mTileMesh->SetMobility(EComponentMobility::Movable);
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> tileMesh(TEXT("StaticMesh'/Game/OneWars/Player/HexTile_mesh.HexTile_mesh'"));
	//auto* mesh = tileMesh.Object;

	//mTileMesh->SetStaticMesh(mesh);
	//FRotator rotation = FRotator(0, 90, 0);
	//mTileMesh->SetRelativeRotation(rotation);
	/*mSize = rotation.RotateVector(mesh->GetBoundingBox().GetSize());*/
}

void UOWHexTile::GenerateMesh()
{
	mSize = FVector(mTileSize, mTileSize, 0.f);
	mBorderVertices.Empty();
	mBorderTriangles.Empty();
	CreateHex();
}

const FVector& UOWHexTile::GetSize()
{
	return mSize;
}

UProceduralMeshComponent* UOWHexTile::GetTileMesh()
{
	return mTileMesh;
}

UProceduralMeshComponent* UOWHexTile::GetBorderMesh()
{
	return mBorderMesh;
}

void UOWHexTile::SetRelativeLocation(const FVector& localLocation)
{
	Root->SetRelativeLocation(localLocation);
}

void UOWHexTile::SetAsSelected(bool selected)
{
	if (selected)
	{
		mTileMesh->SetMaterial(0, mSelectedMaterial);
	}
	else
	{
		mTileMesh->SetMaterial(0, mDefaultMaterial);
	}
}

void UOWHexTile::CreateForm(const FVector& centerPosition, int32 size, TArray<FVector>& vertices, TArray<int32>& triangles)
{
	auto firstIndex = vertices.Num();

	int32 current = 0;
	//A
	{
		triangles.Add(firstIndex + current);
		triangles.Add(firstIndex + current + 3);
		triangles.Add(firstIndex + current + 1);
	}
	{
		triangles.Add(firstIndex + current);
		triangles.Add(firstIndex + current + 2);
		triangles.Add(firstIndex + current + 3);
	}
	current = 2;
	//B
	{
		triangles.Add(firstIndex + current);
		triangles.Add(firstIndex + current + 3);
		triangles.Add(firstIndex + current + 1);
	}
	{
		triangles.Add(firstIndex + current);
		triangles.Add(firstIndex + current + 2);
		triangles.Add(firstIndex + current + 3);
	}

	current = 4;
	//C
	{
		triangles.Add(firstIndex + current);
		triangles.Add(firstIndex + current + 3);
		triangles.Add(firstIndex + current + 1);
	}
	{
		triangles.Add(firstIndex + current);
		triangles.Add(firstIndex + current + 2);
		triangles.Add(firstIndex + current + 3);
	}

	current = 6;
	//D
	{
		triangles.Add(firstIndex + current);
		triangles.Add(firstIndex + current + 3);
		triangles.Add(firstIndex + current + 1);
	}
	{
		triangles.Add(firstIndex + current);
		triangles.Add(firstIndex + current + 2);
		triangles.Add(firstIndex + current + 3);
	}

	current = 8;
	//E
	{
		triangles.Add(firstIndex + current);
		triangles.Add(firstIndex + current + 3);
		triangles.Add(firstIndex + current + 1);
	}
	{
		triangles.Add(firstIndex + current);
		triangles.Add(firstIndex + current + 2);
		triangles.Add(firstIndex + current + 3);
	}


	//F
	{
		triangles.Add(firstIndex + 10);
		triangles.Add(firstIndex + 1);
		triangles.Add(firstIndex + 11);
	}
	{
		triangles.Add(firstIndex + 10);
		triangles.Add(firstIndex + 0);
		triangles.Add(firstIndex + 1);
	}


	int32 sides = 6;
	int32 outerSize = size;
	int32 innerSize = size * 0.9f;

	for (int32 i = 0; i < sides; i++)
	{
		auto angleDeg = 60 * i - 30;
		auto angleRad = PI / 180 * angleDeg;

		//Inner Hex vertices
		FVector innerVertice = { centerPosition.X + innerSize * FMath::Cos(angleRad), centerPosition.Y + innerSize * FMath::Sin(angleRad), centerPosition.Z };
		vertices.Add(innerVertice);

		//Outer Hex vertices
		FVector outerVertice = { centerPosition.X + outerSize * FMath::Cos(angleRad), centerPosition.Y + outerSize * FMath::Sin(angleRad), centerPosition.Z };
		vertices.Add(outerVertice);
	}
}

void UOWHexTile::CreateHex()
{
	auto halfTile = mTileSize / 2;
	CreateForm(FVector(halfTile, halfTile, 0.f), mTileSize, mBorderVertices, mBorderTriangles);
}

UMaterialInstanceDynamic* UOWHexTile::CreateMaterialInstance(const FColor& color, float opacity)
{
	auto* material = UMaterialInstanceDynamic::Create(mMasterMaterial, this);
	material->SetVectorParameterValue("Color", color);
	material->SetScalarParameterValue("Opacity", opacity);
	return material;
}