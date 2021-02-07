// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "RumbleGameInstance.h"
#include "Components/Button.h"


bool UInGameMenu::Initialize()
{
	const bool Success = Super::Initialize();
	if(!Success) return false;

	if (!ensure(ResumeBtn != nullptr)) return false;
	ResumeBtn->OnClicked.AddDynamic(this, &UInGameMenu::ResumePressed);

	if (!ensure(QuitBtn != nullptr)) return false;
	QuitBtn->OnClicked.AddDynamic(this, &UInGameMenu::QuitPressed);

	return true;
}


void UInGameMenu::QuitPressed()
{
	if (MenuInterface != nullptr)
	{
		GameFocus();
		MenuInterface->LoadMainMenu();
	}
	
}void UInGameMenu::ResumePressed()
{
	if (MenuInterface != nullptr)
	{
		GameFocus();
	}
}



