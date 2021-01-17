// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UI/Nameplate.h"
#include "OneWarsCharacter.generated.h"

UCLASS(Blueprintable)
class AOneWarsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AOneWarsCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	void CameraUnlockToggle();

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }

	/** Returns UnlockedCamera subobject **/
	FORCEINLINE class UCameraComponent* GetUnlockedCamera() const { return UnlockedCamera; }

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	FORCEINLINE class UWidgetComponent* GetNameplate() { return mNameplateComponent; }

	//FORCEINLINE class UNetworkedChatSystemActor* GetChatActor() { return mChatComponent; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString mCharacterName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 mLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float mHealth;

	UFUNCTION(BlueprintCallable)
	const FString& GetCharacterName();

	UFUNCTION(BlueprintCallable)
	const int32 GetLevel();

	UFUNCTION(BlueprintCallable)
	const float GetHealth();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
	TSubclassOf<UNameplate> NameplateClass;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Unlocked camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* UnlockedCamera;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Game, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* mNameplateComponent;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Chat, meta = (AllowPrivateAccess = "true"))
	class UNetworkedChatSystemActor* mChatACtor;*/

	void ConstructNameplate();

	bool mCameraLock;

	bool FlipFloop(bool& toBeFlipped);
};

