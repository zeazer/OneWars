// Copyright Epic Games, Inc. All Rights Reserved.

#include "OneWarsCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "OneWarsGameMode.h"
#include "OWGrid.h"
#include "OwHexGrid.h"
#include "Components/WidgetComponent.h"
#include "OneWarsGameInstance.h"


AOneWarsCharacter::AOneWarsCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, -90.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create unlocked camera.
	UnlockedCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("UnlockedCamera"));
	UnlockedCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	UnlockedCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/OneWars/Player/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.§§
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	mNameplateComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameplateComponent"));
	mNameplateComponent->SetupAttachment(RootComponent);

	mCharacterName = TEXT("Player");

	mLevel = 1;

	mHealth = 1.f;

	mCameraLock = true;
}

void AOneWarsCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			if (TraceHitResult.bBlockingHit)
			{
				FVector CursorFV = TraceHitResult.ImpactNormal;
				FRotator CursorR = CursorFV.Rotation();
				auto hitLocation = TraceHitResult.Location;
				CursorToWorld->SetWorldLocation(hitLocation);
				CursorToWorld->SetWorldRotation(CursorR);
			}

		}
	}
}

void AOneWarsCharacter::OnConstruction(const FTransform& Transform)
{
	ConstructNameplate();
}

void AOneWarsCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (mNameplateComponent)
	{
		auto* widget = mNameplateComponent->GetUserWidgetObject();
		auto* nameplate = Cast<UNameplate>(widget);
		if (nameplate)
		{
			nameplate->mCharacter = this;
		}
	}
	if (UnlockedCamera)
	{
		UnlockedCamera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void AOneWarsCharacter::CameraUnlockToggle()
{
	static bool cameraUnlock = false;
	FlipFloop(cameraUnlock);
	TopDownCameraComponent->SetActive(!cameraUnlock);
	UnlockedCamera->SetActive(cameraUnlock);
	if (cameraUnlock)
	{
		FVector newLocation = TopDownCameraComponent->GetComponentLocation();
		UnlockedCamera->SetWorldLocation(newLocation);

	}
}

const FString& AOneWarsCharacter::GetCharacterName()
{
	return mCharacterName;
}

const int32 AOneWarsCharacter::GetLevel()
{
	return mLevel;
}

const float AOneWarsCharacter::GetHealth()
{
	return mHealth;
}

void AOneWarsCharacter::ConstructNameplate()
{
	if (NameplateClass)
	{
		mNameplateComponent->SetWidgetClass(NameplateClass);
	}
}

bool AOneWarsCharacter::FlipFloop(bool& toBeFlipped)
{
	toBeFlipped = !toBeFlipped;
	return toBeFlipped;
}
