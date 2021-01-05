// Fill out your copyright notice in the Description page of Project Settings.


#include "OneWarsGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/MenuBase.h"
#include "Kismet/GameplayStatics.h"
#include "OneWarsCharacter.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UOneWarsGameInstance::UOneWarsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	mCurrentActiveWidget = nullptr;

	static ConstructorHelpers::FClassFinder<UMenuBase> MainMenuWidget(TEXT("WidgetBlueprint'/Game/OneWars/UI/MainMenu_BP.MainMenu_BP_C'"));
	if (MainMenuWidget.Succeeded())
	{
		auto type = MainMenuWidget.Class.GetDefaultObject()->mMenuType;

		mMenuWidgetClasses.Add(type, MainMenuWidget.Class);
	}


	static ConstructorHelpers::FClassFinder<UMenuBase> inGameWIdget(TEXT("WidgetBlueprint'/Game/OneWars/UI/InGameHUD.InGameHUD_C'"));
	if (inGameWIdget.Succeeded())
	{
		auto type = inGameWIdget.Class.GetDefaultObject()->mMenuType;

		mMenuWidgetClasses.Add(type, inGameWIdget.Class);
	}
}

void UOneWarsGameInstance::Init()
{

	if (IOnlineSubsystem* subsystem = IOnlineSubsystem::Get())
	{
		mSessionInterface = subsystem->GetSessionInterface();
		if (mSessionInterface.IsValid())
		{
			//Bind Delegates Here!
			mSessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UOneWarsGameInstance::OnCreateSessionComplete);

			mSessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UOneWarsGameInstance::OnFindSessionComplete);

			mSessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UOneWarsGameInstance::OnJoinSessionComplete);
		}
	}

	for (const auto& pair : mMenuWidgetClasses)
	{
		UE_LOG(LogTemp, Warning, TEXT("We created class %s"), *pair.Value->GetName());
	}
	mMenuStack.Push(EMenuType::Default);
}

void UOneWarsGameInstance::PushMenu(EMenuType menuType)
{
	if (mMenuStack.Last() != menuType)
	{
		mMenuStack.Push(menuType);
	}
	ShowWidget();
}

void UOneWarsGameInstance::PopMenu()
{
	RemoveCurrentWidget();
	mMenuStack.Pop();
	ShowWidget();
}

void UOneWarsGameInstance::SwitchToLevelWithName(const FString& name, const FName& levelName)
{
	if (!name.IsEmpty())
	{
		auto* world = GetWorld();
		if (world->GetMapName() != levelName.ToString())
		{
			PopMenu();
			UGameplayStatics::OpenLevel(GetWorld(), levelName);
		}
	}
}

const FString& UOneWarsGameInstance::GetUsername()
{
	return mUsername;
}

void UOneWarsGameInstance::OnCreateSessionComplete(FName ServerName, bool Succeeded)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete Succeeded: %d"), Succeeded);
	if (Succeeded)
	{
		GetWorld()->ServerTravel("/Game/OneWars/Maps/OneWarsWorld?listen");
	}
}

void UOneWarsGameInstance::OnFindSessionComplete(bool Succeeded)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete Succeeded: %d"), Succeeded);
	if (Succeeded)
	{
		auto searchResults = mSessionSearch->SearchResults;
		UE_LOG(LogTemp, Warning, TEXT("Search Results, Server Count: %d"), searchResults.Num());
		if (searchResults.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("Joining Server"));
			mSessionInterface->JoinSession(0, "My Session", searchResults[0]);
		}
	}
}

void UOneWarsGameInstance::OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete"));
	if (auto* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAdress = "";
		mSessionInterface->GetResolvedConnectString(sessionName, JoinAdress);
		if (!JoinAdress.IsEmpty())
		{
			playerController->ClientTravel(JoinAdress, ETravelType::TRAVEL_Absolute);
		}
	}
}

void UOneWarsGameInstance::CreateServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Create Server"));
	FOnlineSessionSettings sessionSettings;
	sessionSettings.bAllowJoinInProgress = true;
	sessionSettings.bIsDedicated = false;
	sessionSettings.bIsLANMatch = true;
	sessionSettings.bShouldAdvertise = true;
	sessionSettings.bUsesPresence = true;
	sessionSettings.NumPublicConnections = 5;

	mSessionInterface->CreateSession(0, FName("My Session"), sessionSettings);
}

void UOneWarsGameInstance::JoinServer()
{
	mSessionSearch = MakeShareable(new FOnlineSessionSearch());
	mSessionSearch->bIsLanQuery = true;
	mSessionSearch->MaxSearchResults = 10000;
	mSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	mSessionInterface->FindSessions(0, mSessionSearch.ToSharedRef());
}

void UOneWarsGameInstance::ShowWidget()
{
	if (mMenuStack.Num() < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("No menus on the stack"));
		return;
	}
	auto* typeInMap = mMenuWidgetClasses.Find(mMenuStack.Last());

	if (!typeInMap)
	{
		UE_LOG(LogTemp, Warning, TEXT("A menu wasn't created or didn't exist"));
		return;
	}

	auto widgetClass = typeInMap->Get();
	auto* menu = CreateWidget<UMenuBase>(this, widgetClass);
	menu->AddToViewport();

	menu->Init();

	mCurrentActiveWidget = menu;
}

void UOneWarsGameInstance::RemoveCurrentWidget()
{
	mCurrentActiveWidget->RemoveFromParent();
	mCurrentActiveWidget = nullptr;
}
