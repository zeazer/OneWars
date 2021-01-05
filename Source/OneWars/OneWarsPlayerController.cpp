// Copyright Epic Games, Inc. All Rights Reserved.

#include "OneWarsPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "OneWarsCharacter.h"
#include "Engine/World.h"
#include "OneWarsGameMode.h"
#include "OWGrid.h"
#include "OWHexGrid.h"
#include "OneWarsGameInstance.h"
#include "Engine/Engine.h"
#include "PlayerHUD.h"

AOneWarsPlayerController::AOneWarsPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	MyHUD = CreateDefaultSubobject<APlayerHUD>(TEXT("HUD"));
}

void AOneWarsPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AOneWarsPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void AOneWarsPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AOneWarsPlayerController::MoveToMouseCursor);
	//	InputComponent->BindAction("SetDestination", IE_Released, this, &AOneWarsPlayerController::OnSetDestinationReleased);

		// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AOneWarsPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AOneWarsPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AOneWarsPlayerController::OnResetVR);
}

void AOneWarsPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AOneWarsPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (AOneWarsCharacter* MyPawn = Cast<AOneWarsCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		auto location = Hit.ImpactPoint;

		/*auto* grid = Cast<AOneWarsGameMode>(GetWorld()->GetAuthGameMode<AGameModeBase>())->GetHexGrid();*/


		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			/*if (grid)
			{
				FVector centerLocation = grid->GetTilePosition(Hit.Item);
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Grid Location: %s"), *centerLocation.ToString()));
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Cursor Location: %s"), *location.ToString()));
			}*/
			//SetNewMoveDestination({ centerLocation.X, centerLocation.Y, location.Z });
			SetNewMoveDestination(location);
		}
	}
}

void AOneWarsPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		auto location = HitResult.ImpactPoint;

		auto* grid = Cast<AOneWarsGameMode>(GetWorld()->GetAuthGameMode<AGameModeBase>())->GetHexGrid();

		bool isValid = true;
		int32 row = -1;
		int32 column = -1;
		// We hit something, move there
		if (isValid)
		{
			FVector centerLocation = grid->GetTilePosition(HitResult.Item);

			SetNewMoveDestination({ centerLocation.X, centerLocation.Y, location.Z });
		}
	}
}

void AOneWarsPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AOneWarsPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AOneWarsPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}
