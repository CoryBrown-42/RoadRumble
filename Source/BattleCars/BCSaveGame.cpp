// Fill out your copyright notice in the Description page of Project Settings.


#include "BCSaveGame.h"
#include "Kismet/GameplayStatics.h"

UBCSaveGame::UBCSaveGame()
{
	SaveSlotName = TEXT("Save1");
	UserIndex = 0;
}

void UBCSaveGame::SaveGame()
{
	PlayerName = TEXT("PlayerOne");
	UBCSaveGame* SaveGameInstance = Cast<UBCSaveGame>(UGameplayStatics::CreateSaveGameObject(UBCSaveGame::StaticClass()));
	SaveGameInstance->PlayerName = PlayerName;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
}

void UBCSaveGame::LoadGame() 
{
	UBCSaveGame* LoadGameInstance = Cast<UBCSaveGame>(UGameplayStatics::CreateSaveGameObject(UBCSaveGame::StaticClass()));
	LoadGameInstance = Cast<UBCSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
	const FString PlayerNameToDisplay = LoadGameInstance->PlayerName;
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, PlayerNameToDisplay);
	}
}

