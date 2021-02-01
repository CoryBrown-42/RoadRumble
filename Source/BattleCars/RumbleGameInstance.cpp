// Fill out your copyright notice in the Description page of Project Settings.


#include "RumbleGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "Trigger.h"
#include "MainMenu.h"

URumbleGameInstance::URumbleGameInstance(const FObjectInitializer& ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("Game Init"));

	//Initialize the main menu reference to the widget blueprint.
	ConstructorHelpers::FClassFinder<UUserWidget>MenuBPClass(TEXT("/Game/UI/MainMenu"));

	//Check if the widget is not null
	if (!ensure(MenuBPClass.Class != nullptr)) return;
	MenuClass = MenuBPClass.Class;

}

void URumbleGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Instance Init"));
}

void URumbleGameInstance::LoadMenu()
{
	if (!ensure(MenuClass != nullptr)) return;
	UMainMenu* Menu = CreateWidget<UMainMenu>(this, MenuClass);

	Menu->Setup();
	
	Menu->SetMenuInterface(this);

}

//Command to Host a game.
void URumbleGameInstance::Host()
{
	if (!ensure(MenuClass != nullptr)) return;
	UMainMenu* Menu = CreateWidget<UMainMenu>(this, MenuClass);

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