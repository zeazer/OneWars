// Fill out your copyright notice in the Description page of Project Settings.


#include "OWGrid.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "ProceduralMeshComponent.h"
#include "OneWarsGameMode.h"
#include "Engine/GameEngine.h"

// Sets default values
AOWGrid::AOWGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mNumberOfRows = 10;
	mNumberOfColumns = 10;
	mTileSize = 10.f;
	mLineThickness = 10.f;
	mLineColor = FColor::Red;
	mSelectionColor = FColor::Green;
	mLineOpacity = 1.f;
	mSelectionOpacity = 1.f;


	static ConstructorHelpers::FObjectFinder<UMaterial> materialAsset(TEXT("Material'/Game/OneWars/Materials/GridMasterMaterial.GridMasterMaterial'"));
	if (materialAsset.Succeeded())
	{
		mMasterMaterial = materialAsset.Object;
	}

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	mGridMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GridMesh"));
	mGridMesh->SetupAttachment(Root);

	mSelectionMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("SelectionMesh"));
	mSelectionMesh->SetupAttachment(Root);
}

void AOWGrid::OnConstruction(const FTransform& Transform)
{
	UpdateMesh();
}

void AOWGrid::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UpdateMesh();
}

void AOWGrid::BeginPlay()
{
	Super::BeginPlay();
	Cast<AOneWarsGameMode>(GetWorld()->GetAuthGameMode<AGameModeBase>())->SetGrid(this);
}

void AOWGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOWGrid::LocationToTile(const FVector& worldLocation, bool& isValid, int32& row, int32& column)
{
	float worldPosX = worldLocation.X;
	float gridPosX = GetActorLocation().X;

	row = FMath::FloorToInt((worldPosX - gridPosX) / GetGridHeight() * mNumberOfRows);

	float worldPosY = worldLocation.Y;
	float gridPosY = GetActorLocation().Y;

	column = FMath::FloorToInt((worldPosY - gridPosY) / GetGridWitdh() * mNumberOfColumns);

	isValid = IsTileValid(row, column);
}

void AOWGrid::TileToGridLocation(int32 row, int32 column, bool isCenter, bool& isValid, FVector2D& gridLcoation)
{
	isValid = IsTileValid(row, column);
	auto actorLocation = GetActorLocation();

	float offset = isCenter ? mTileSize / 2 : 0;

	float x = (row * mTileSize) + actorLocation.X + offset;

	float y = (column * mTileSize) + actorLocation.Y + offset;

	gridLcoation.X = x;
	gridLcoation.Y = y;
}

void AOWGrid::SetSelectedTile(int32 row, int32 column)
{
	bool isValid = false;
	FVector2D gridLocation = FVector2D::ZeroVector;
	TileToGridLocation(row, column, false, isValid, gridLocation);

	if (isValid)
	{
		mSelectionMesh->SetVisibility(true);
		FVector location = GetActorLocation();
		location.X = gridLocation.X;
		location.Y = gridLocation.Y;
		mSelectionMesh->SetWorldLocation(location);
	}
	else
	{
		mSelectionMesh->SetVisibility(false);
	}
}

void AOWGrid::CreateVerticalLines()
{
	auto lineEnd = GetGridHeight();
	for (int32 i = 0; i < mNumberOfColumns + 1; i++)
	{
		auto lineStart = i * mTileSize;
		CreateLine(FVector(0.f, lineStart, 0.f), FVector(lineEnd, lineStart, 0.f), mLineThickness, mLineVertices, mLineTriangles);
	}
}

void AOWGrid::CreateHorizontalLines()
{
	auto lineEnd = GetGridWitdh();
	for (int32 i = 0; i < mNumberOfRows + 1; i++)
	{
		auto lineStart = i * mTileSize;
		CreateLine(FVector(lineStart, 0.f, 0.f), FVector(lineStart, lineEnd, 0.f), mLineThickness, mLineVertices, mLineTriangles);
	}
}

void AOWGrid::CreateLine(const FVector& startPosition, const FVector& endPosition, float thickness, TArray<FVector>& vertices, TArray<int32>& triangles)
{
	auto halfThickness = thickness / 2.f;
	auto thicknessDirection = FVector::CrossProduct((endPosition - startPosition).GetSafeNormal(), FVector(0.f, 0.f, 1.f));

	auto lenght = vertices.Num();

	auto bottomLeft = lenght + 2;
	auto topRight = lenght + 1;
	auto topLeft = lenght;
	auto bottomRight = lenght + 3;

	triangles.Append({ bottomLeft,  topRight ,topLeft , bottomLeft ,bottomRight ,topRight });

	auto thicky = thicknessDirection * halfThickness;

	auto vertex0 = startPosition + thicky;
	auto vertex1 = endPosition + thicky;
	auto vertex2 = startPosition - thicky;
	auto vertex3 = endPosition - thicky;

	vertices.Append({ vertex0, vertex1, vertex2, vertex3 });
}

float AOWGrid::GetGridWitdh() const
{
	return mNumberOfColumns * mTileSize;
}

float AOWGrid::GetGridHeight() const
{
	return mNumberOfRows * mTileSize;
}

UMaterialInstanceDynamic* AOWGrid::CreateMaterialInstance(const FColor& color, float opacity)
{
	auto* material = UMaterialInstanceDynamic::Create(mMasterMaterial, this);
	material->SetVectorParameterValue("Color", color);
	material->SetScalarParameterValue("Opacity", opacity);
	return material;
}

void AOWGrid::UpdateMesh()
{
	if (mGridMesh)
	{
		CreateGridMesh();
	}
	if (mSelectionMesh)
	{
		CreateSelectionMesh();
	}
}


void AOWGrid::CreateGridMesh()
{
	mLineVertices.Empty();
	mLineTriangles.Empty();
	mLineMaterial = CreateMaterialInstance(mLineColor, mLineOpacity);
	CreateVerticalLines();
	CreateHorizontalLines();
	mGridMesh->CreateMeshSection(0, mLineVertices, mLineTriangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);

	mGridMesh->SetMaterial(0, mLineMaterial);
}

void AOWGrid::CreateSelectionMesh()
{
	mSelectionVertices.Empty();
	mSelectionTriangles.Empty();
	mSelectedMaterial = CreateMaterialInstance(mSelectionColor, mSelectionOpacity);
	auto halfTile = mTileSize / 2;
	CreateLine(FVector(0.f, halfTile, 0.f), FVector(mTileSize, halfTile, 0.f), mTileSize, mSelectionVertices, mSelectionTriangles);

	mSelectionMesh->SetVisibility(false);
	mSelectionMesh->CreateMeshSection(0, mSelectionVertices, mSelectionTriangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	mSelectionMesh->SetMaterial(0, mSelectedMaterial);
}

bool AOWGrid::IsTileValid(int32 row, int32 column)
{
	return (row >= 0 && row < mNumberOfRows) && (column >= 0 && column < mNumberOfColumns);
}

