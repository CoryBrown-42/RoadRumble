// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	//TODO: setup
	if (!ensure(HostBtn != nullptr)) return false;
	HostBtn->OnClicked.AddDynamic(this, &UMainMenu::HostServer);


	return true;

}

void UMainMenu::SetMenuInterface(IMenuInterface* MMenuInterface)
{
	MenuInterface = MMenuInterface;
}

void UMainMenu::Setup()
{

	this->AddToViewport(0);
	this->bIsFocusable = true;
	/*Handle the the behavior of the widget we adding to the screen.*/

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

void UMainMenu::GameFocus()
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


void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->Host();
	}
}