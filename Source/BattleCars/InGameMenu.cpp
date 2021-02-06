// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "RumbleGameInstance.h"
#include "Components/Button.h"


bool UInGameMenu::Initialize()
{
	const bool Success = Super::Initialize();
	if(!Success) return false;

	if (!ensure(ResumeBtn != nullptr)) return false;
	ResumeBtn->OnClicked.AddDynamic(this, &UInGameMenu::GameFocus);

	if (!ensure(QuitBtn != nullptr)) return false;
	QuitBtn->OnClicked.AddDynamic(this, &UInGameMenu::GoToMainMenu);

	return true;
}


void UInGameMenu::GoToMainMenu()
{
	
	UEngine* Engine = GetGameInstance()->GetEngine();
	if (!ensure(Engine != nullptr)) return;
	UE_LOG(LogTemp, Warning, TEXT("Game Instance Init"));
	Engine->AddOnScreenDebugMessage(1, 2, FColor::Green, TEXT("Leaving"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Maps/MainMenu?listen");
	
}

void UInGameMenu::SetMenuInterface(IMenuInterface* MMenuInterface)
{
	MenuInterface = MMenuInterface;
}

void UInGameMenu::Setup()
{
	//if (!ensure(this != nullptr)) return;
	this->AddToViewport(0);
	this->bIsFocusable = true;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UInGameMenu::GameFocus()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	// Sets back InputMode to Game Only
	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;

}


