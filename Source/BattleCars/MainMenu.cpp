// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "RumbleGameInstance.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"



bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	//TODO: setup
	if (!ensure(HostBtn != nullptr)) return false;
	HostBtn->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinBtn != nullptr)) return false;
	JoinBtn->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(BackBtn != nullptr)) return false;
	BackBtn->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(SubJoinBtn != nullptr)) return false;
	SubJoinBtn->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!ensure(QuitBtn != nullptr)) return false;
	QuitBtn->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

	return true;

}


void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->Host();
	}
}

void UMainMenu::QuitGame()
{
	
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("Quit");
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::JoinServer()
{
	if (MenuInterface != nullptr)
	{
		if (!ensure(IPAddressField != nullptr)) return;
		const FString& Address = IPAddressField->GetText().ToString();
		MenuInterface->Join(Address);
	}
}
