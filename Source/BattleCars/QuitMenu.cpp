// Fill out your copyright notice in the Description page of Project Settings.


#include "QuitMenu.h"
#include "RumbleGameInstance.h"
#include "Components/Button.h"


bool UQuitMenu::Initialize()
{
	const bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(NoBtn != nullptr)) return false;
	NoBtn->OnClicked.AddDynamic(this, &UQuitMenu::NoPressed);

	if (!ensure(YesBtn != nullptr)) return false;
	YesBtn->OnClicked.AddDynamic(this, &UQuitMenu::YesPressed);

	return true;
}


void UQuitMenu::YesPressed()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("Quit");
}

void UQuitMenu::NoPressed()
{
	if (MenuInterface != nullptr)
	{
		TearDown();
	}
}