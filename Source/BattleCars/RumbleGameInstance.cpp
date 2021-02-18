// Fill out your copyright notice in the Description page of Project Settings.


#include "RumbleGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Trigger.h"
#include "MainMenu.h"
#include "InGameMenu.h"
#include "QuitMenu.h"
#include "BCSaveGame.h"

const static FName SESSION_NAME = TEXT("Game Session");


URumbleGameInstance::URumbleGameInstance(const FObjectInitializer& ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("Game Init"));

	//Initialize the main menu reference to the widget blueprint.
	ConstructorHelpers::FClassFinder<UUserWidget>MenuBPClass(TEXT("/Game/UI/MainMenu"));
	//Check if the widget is not null
	if (!ensure(MenuBPClass.Class != nullptr)) return;
	MenuClass = MenuBPClass.Class;


	//Initialize the in-game menu reference to the widget blueprint.
	ConstructorHelpers::FClassFinder<UUserWidget>InGameMenuBPClass(TEXT("/Game/UI/InGameMenu"));
	//Check if the widget is not null
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;
	InGameMenuClass = InGameMenuBPClass.Class;

	/*Initialize the quit menu*/
	ConstructorHelpers::FClassFinder<UUserWidget>QuitMenuBPClass(TEXT("/Game/UI/QuitMenu"));
	//Check if the widget is not null
	if (!ensure(QuitMenuBPClass.Class != nullptr)) return;
	QuitMenuClass = QuitMenuBPClass.Class;

}

/*Initialize the game instance.
Checks if the online subsystem isn't null and log that the Subsystem has been found.
If there is already a session - destroy it*/
void URumbleGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Instance Init"));
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Subsystem %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &URumbleGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &URumbleGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &URumbleGameInstance::OnFindSessionsComplete);
		}

	}
	else
	{
		//No subsystem is was found.
		UE_LOG(LogTemp, Warning, TEXT("Found No Subsystem"));
	}
	
}

void URumbleGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass != nullptr)) return;
	UMainMenu* Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;
	//Setup Menu
	Menu->Setup();
	Menu->SetMenuInterface(this);

}

void URumbleGameInstance::InGameLoadMenu()
{
	if (!ensure(InGameMenuClass != nullptr)) return;
	UInGameMenu* GameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);

	GameMenu->Setup();
	GameMenu->SetMenuInterface(this);
}

void URumbleGameInstance::LoadQuitMenu()
{
	if (!ensure(QuitMenuClass != nullptr)) return;
	UQuitMenu* QuitMenu = CreateWidget<UQuitMenu>(this, QuitMenuClass);
	if (QuitMenu != nullptr)
	{
		QuitMenu->Setup();
		QuitMenu->SetMenuInterface(this);
	}
	
}

//Command to Host a game.
void URumbleGameInstance::Host()
{
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

/*Creates a session and open map*/
void URumbleGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!ensure(MenuClass != nullptr)) return;
	UMainMenu* Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!Success) 
	{
		UE_LOG(LogTemp, Error, TEXT("Could not create session."));
		return;
	}

	if (Menu != nullptr)
	{
		Menu->GameFocus();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(1, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	//Travel to world map as listen server.
	World->ServerTravel("/Game/Maps/TestMap?listen");

}

void URumbleGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void URumbleGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;
		//Lan Query for now but we'll need to set up Steam
		//SessionSearch->QuerySettings.Set
		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void URumbleGameInstance::OnFindSessionsComplete(bool Success)
{	
	if (!ensure(MenuClass != nullptr)) return;
	UMainMenu* Menu = CreateWidget<UMainMenu>(this, MenuClass);

	if (Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Finished Finding Session"));

		TArray<FString> ServerNames;

		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Session name: %s"), *SearchResult.GetSessionIdStr());
			ServerNames.Add(SearchResult.GetSessionIdStr());
		}
		Menu->SetServerList(ServerNames);
	}
}

//Command to Join a game.
void URumbleGameInstance::Join(const FString& Address)
{
	if (!ensure(MenuClass != nullptr)) return;
	UMainMenu* Menu = CreateWidget<UMainMenu>(this, MenuClass);

	if (Menu != nullptr)
	{
		Menu->SetServerList({ "Test1", "Test2" });
	}
	//UEngine* Engine = GetEngine();
	//if (!ensure(Engine != nullptr)) return;
	//Engine->AddOnScreenDebugMessage(1, 5, FColor::Green, FString::Printf(TEXT("Joinging %s"), *Address));
	//
	//APlayerController* PlayerController = GetFirstLocalPlayerController();
	//if (!ensure(PlayerController != nullptr)) return;
	//
	//PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void URumbleGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;

		//Session Settings
		SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 6;
		SessionSettings.bShouldAdvertise = true;

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
	
}



//Command to Kill yourself.
void URumbleGameInstance::Kill()
{
	GetPrimaryPlayerController(true)->GetOwner()->Destroy();
}

void URumbleGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}