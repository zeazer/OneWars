// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHexGrid.h"
#include "OWHexTile.h"
#include "ProceduralMeshComponent.h"

// Sets default values
AOWHexGrid::AOWHexGrid() :
	mRows(5),
	mColumns(1)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root);
}

void AOWHexGrid::CreateGrid()
{
	if (auto* world = GetWorld())
	{
		for (int32 x = 0; x < mColumns; x++)
		{
			for (int32 y = 0; y < mRows; y++)
			{
				UOWHexTile* tile = NewObject<UOWHexTile>(Root, UOWHexTile::StaticClass());
				tile->GenerateMesh();
				tile->RegisterComponentWithWorld(world);
				auto size = tile->GetBorderMesh()->GetNavigationBounds().GetSize();
				tile->SetRelativeLocation(FVector(x * size.X + (0.5 * size.X * (y % 2)), y * size.Y * 3/4,0));
			}
		}
	}
}

void AOWHexGrid::OnConstruction(const FTransform& Transform)
{
	CreateGrid();
}

void AOWHexGrid::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	CreateGrid();
}