// Fill out your copyright notice in the Description page of Project Settings.


#include "OWGrid.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"
#include "ProceduralMeshComponent.h"
#include "OneWarsGameMode.h"
#include "Engine/GameEngine.h"
#include "Math/UnrealMathVectorConstants.h"

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

	float x = (row * mTileSize * sqrt(3)) + actorLocation.X;

	float y = (column * ((mTileSize*2)*(3/4))) + actorLocation.Y;

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
		mSelectionMesh->SetVisibility(true);
	}
}

void AOWGrid::CreateVerticalLines()
{
	auto lineEnd = GetGridHeight();
	for (int32 i = 0; i < mNumberOfColumns + 1; i++)
	{
		auto lineStart = i * mTileSize;
		CreateLine(FVector(0.f, lineStart, 0.f), FVector(lineEnd, lineStart, 0.f), mLineThickness, mGridVertices, mGridTriangles);
	}
}

void AOWGrid::CreateHorizontalLines()
{
	auto lineEnd = GetGridWitdh();
	for (int32 i = 0; i < mNumberOfRows + 1; i++)
	{
		auto lineStart = i * mTileSize;
		CreateLine(FVector(lineStart, 0.f, 0.f), FVector(lineStart, lineEnd, 0.f), mLineThickness, mGridVertices, mGridTriangles);
	}
}

void AOWGrid::CreateHexagonGrid()
{
	int32 width = mTileSize * sqrt(3);
	int32 height = mTileSize * 2;
	for (int32 x = 0; x < mNumberOfColumns; x++)
	{
		for (int32 y = 0; y < mNumberOfRows; y++)
		{
			CreateForm(FVector(x * width + (0.5 * width * (y % 2)), y * height * 0.75f, 0), mTileSize, mLineThickness, mGridVertices, mGridTriangles);
		}
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

	triangles.Append({ bottomLeft, topRight, topLeft, bottomLeft, bottomRight, topRight });

	auto thicky = thicknessDirection * halfThickness;

	auto vertex0 = startPosition + thicky;
	auto vertex1 = endPosition + thicky;
	auto vertex2 = startPosition - thicky;
	auto vertex3 = endPosition - thicky;

	vertices.Append({ vertex0, vertex1, vertex2, vertex3 });
}

void AOWGrid::CreateForm(const FVector& centerPosition, int32 size, float thickness, TArray<FVector>& vertices, TArray<int32>& triangles)
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

void AOWGrid::CreateFilledHex(const FVector& centerPosition, int32 size, float thickness, TArray<FVector>& vertices, TArray<int32>& triangles)
{
	auto firstIndex = vertices.Num();

	{
		triangles.Add(firstIndex);
		triangles.Add(firstIndex + 2);
		triangles.Add(firstIndex + 1);
	}

	{
		triangles.Add(firstIndex);
		triangles.Add(firstIndex + 3);
		triangles.Add(firstIndex + 2);
	}

	{
		triangles.Add(firstIndex);
		triangles.Add(firstIndex + 4);
		triangles.Add(firstIndex + 3);
	}

	{
		triangles.Add(firstIndex);
		triangles.Add(firstIndex + 5);
		triangles.Add(firstIndex + 4);
	}

	{
		triangles.Add(firstIndex);
		triangles.Add(firstIndex + 6);
		triangles.Add(firstIndex + 5);
	}

	{
		triangles.Add(firstIndex);
		triangles.Add(firstIndex + 1);
		triangles.Add(firstIndex + 6);
	}

	int32 sides = 6;
	vertices.Add(centerPosition);
	for (int32 i = 0; i < sides; i++)
	{
		auto angleDeg = 60 * i - 30;
		auto angleRad = PI / 180 * angleDeg;
		FVector outerVertice = { centerPosition.X + size * FMath::Cos(angleRad), centerPosition.Y + size * FMath::Sin(angleRad), centerPosition.Z };
		vertices.Add(outerVertice);
	}
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
	mGridVertices.Empty();
	mGridTriangles.Empty();
	mLineMaterial = CreateMaterialInstance(mLineColor, mLineOpacity);
	CreateHexagonGrid();
	mGridMesh->CreateMeshSection(0, mGridVertices, mGridTriangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);

	mGridMesh->SetMaterial(0, mLineMaterial);
}

void AOWGrid::CreateSelectionMesh()
{
	mSelectionVertices.Empty();
	mSelectionTriangles.Empty();
	mSelectedMaterial = CreateMaterialInstance(mSelectionColor, mSelectionOpacity);
	CreateFilledHex(FVector(0.0f, 0.0f, 0.f), mTileSize * 0.9f, mLineThickness, mSelectionVertices, mSelectionTriangles);

	mSelectionMesh->SetVisibility(true);
	mSelectionMesh->CreateMeshSection(0, mSelectionVertices, mSelectionTriangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	mSelectionMesh->SetMaterial(0, mSelectedMaterial);
}

bool AOWGrid::IsTileValid(int32 row, int32 column)
{
	return (row >= 0 && row < mNumberOfRows) && (column >= 0 && column < mNumberOfColumns);
}

