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

}

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

		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found No Subsystem"));
	}
	
}

void URumbleGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass != nullptr)) return;
	UMainMenu* Menu = CreateWidget<UMainMenu>(this, MenuClass);

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

void URumbleGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
	
}

//Command to Join a game.
void URumbleGameInstance::Join(const FString& Address)
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(1, 5, FColor::Green, FString::Printf(TEXT("Joinging %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
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