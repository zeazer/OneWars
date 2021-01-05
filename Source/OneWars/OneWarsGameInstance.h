// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/MenuType.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OneWarsGameInstance.generated.h"
/**
 * 
 */
UCLASS()
class ONEWARS_API UOneWarsGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UOneWarsGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void PushMenu(EMenuType menuType);

	UFUNCTION(BlueprintCallable)
	void PopMenu();

	UFUNCTION(BlueprintCallable)
	void SwitchToLevelWithName(const FString& name, const FName& levelName);

	const FString& GetUsername();

protected:

	virtual void OnCreateSessionComplete(FName serverName, bool succeeded);
	virtual void OnFindSessionComplete(bool succeeded);
	virtual void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result);

	UFUNCTION(BlueprintCallable)
	void CreateServer();

	UFUNCTION(BlueprintCallable)
	void JoinServer();

	IOnlineSessionPtr mSessionInterface;

	TSharedPtr<FOnlineSessionSearch> mSessionSearch;
private:
	void ShowWidget();

	void RemoveCurrentWidget();

	TMap<TEnumAsByte<EMenuType>, TSubclassOf<class UMenuBase>> mMenuWidgetClasses;

	TArray<TEnumAsByte<EMenuType>> mMenuStack;

	FString mUsername;

	UUserWidget* mCurrentActiveWidget;
};
