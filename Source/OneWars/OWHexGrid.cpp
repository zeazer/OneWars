// Fill out your copyright notice in the Description page of Project Settings.


#include "OWHexGrid.h"
#include "OWHexTile.h"
#include "OneWarsGameMode.h"
#include "Engine/StaticMesh.h"
#include "Components/SceneComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "ProceduralMeshComponent.h"

AOWHexGrid::AOWHexGrid() :
	mRows(5),
	mColumns(1)
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Root->SetRelativeLocation(FVector::ZeroVector);

	Root->SetMobility(EComponentMobility::Static);

	SetRootComponent(Root);

	mInstancedHexTile = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("HexTileGenerator"));

	mInstancedHexTile->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> tileMesh(TEXT("StaticMesh'/Game/OneWars/Player/HexTile_mesh.HexTile_mesh'"));
	auto* mesh = tileMesh.Object;
	mInstancedHexTile->SetStaticMesh(mesh);
	mInstancedHexTile->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	mInstancedHexTile->RegisterComponentWithWorld(GetWorld());
	mInstancedHexTile->InitPerInstanceRenderData(true);
	mInstancedHexTile->SetFlags(RF_Transactional);
	mInstancedHexTile->SetMobility(EComponentMobility::Static);
	mInstancedHexTile->SetOwnerNoSee(false);
	mInstancedHexTile->SetVisibility(true);

	mRotation = FRotator(0, 90, 0);
	auto size = mesh->GetBoundingBox().GetSize();
	mSize = mRotation.RotateVector(size);
	mInstancedHexTile->SetRelativeLocation(FVector::ZeroVector);
	mInstancedHexTile->bMultiBodyOverlap = true;

	AddInstanceComponent(mInstancedHexTile);

	mCurrentTileIndex = 0;

	mSelectionMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("SelectionMesh"));
	mSelectionMesh->SetupAttachment(Root);
}

FVector AOWHexGrid::GetTilePosition(int32 tileIndex)
{
	FTransform transform;
	mInstancedHexTile->GetInstanceTransform(tileIndex, transform);
	return transform.GetLocation();
}

void AOWHexGrid::SelectTile(int32 index)
{
	auto position = GetTilePosition(index);
	mSelectionMesh->SetVisibility(true);
	mSelectionMesh->SetWorldLocation(position);
}

void AOWHexGrid::CreateGrid()
{
	if (auto* world = GetWorld())
	{
		if (mInstancedHexTile)
		{
			RegisterAllComponents();
			mInstancedHexTile->ClearInstances();

			for (int32 x = 0; x < mColumns; x++)
			{
				for (int32 y = 0; y < mRows; y++)
				{
					auto size = mSize.GetAbs() + 10;
					auto location = FVector(x * size.X + (0.5 * size.X * (y % 2)), y * size.Y * 3 / 4, 0.0f);
					FTransform transform = FTransform(mRotation.Quaternion(), location);
					mInstancedHexTile->AddInstance(transform);
				}
			}
		}
	}
}

void AOWHexGrid::BeginPlay()
{
	Super::BeginPlay();
	Cast<AOneWarsGameMode>(GetWorld()->GetAuthGameMode<AGameModeBase>())->SetHexGrid(this);
	CreateGrid();
}

void AOWHexGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	CreateGrid();
}