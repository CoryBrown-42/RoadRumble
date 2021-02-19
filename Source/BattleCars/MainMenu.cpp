// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "UObject/ConstructorHelpers.h"
#include "RumbleGameInstance.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/UI/ServerRow"));

	if(!ensure(ServerRowBPClass.Class != nullptr)) return;

	ServerRowClass = ServerRowBPClass.Class;
}

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

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	//ServerList->ClearChildren();

	uint32 i = 0;
	for (const FString& ServerName : ServerNames)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->ServerName->SetText(FText::FromString(ServerName));
		Row->Setup(this, i);
		++i;
		ServerList->AddChild(Row);
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index %d"), SelectedIndex.GetValue());
	}
	else
	{

		SelectedIndex = 0;
		UE_LOG(LogTemp, Warning, TEXT("Selected Index not found, joining first found game."));
	}
	if (MenuInterface != nullptr)
	{
		//if (!ensure(IPAddressField != nullptr)) return;
		//const FString& Address = IPAddressField->GetText().ToString();
		MenuInterface->Join("");
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
	if (MenuInterface != nullptr)
	{
		MenuInterface->RefreshServerList();
	}
}


void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitGame()
{	
	URumbleGameInstance* GI = Cast<URumbleGameInstance>(GetGameInstance());
	if(GI != nullptr)
	{
		GI->LoadQuitMenu();
		QuitBtn->bIsEnabled = false;
	}
}







