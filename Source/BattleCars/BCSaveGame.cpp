// Fill out your copyright notice in the Description page of Project Settings.


#include "BCSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UBCSaveGame::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	if (Ar.IsLoading() && SavedDataVersion != SaveGameVersion::LatestVersion)
	{
		if (SavedDataVersion < SaveGameVersion::AddedItemData)
		{
			// Convert from list to item data map
			for (const FPrimaryAssetId& ItemId : InventoryItems_DEPRECATED)
			{
				//InventoryData.Add(ItemId, FItemData(1, 1));
			}

			InventoryItems_DEPRECATED.Empty();
		}

		SavedDataVersion = SaveGameVersion::LatestVersion;
	}
}


//void UBCSaveGame::SaveGame()
//{
//	PlayerName = TEXT("PlayerOne");
//	UBCSaveGame* SaveGameInstance = Cast<UBCSaveGame>(UGameplayStatics::CreateSaveGameObject(UBCSaveGame::StaticClass()));
//	SaveGameInstance->PlayerName = PlayerName;
//	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
//}
//
//void UBCSaveGame::LoadGame() 
//{
//	UBCSaveGame* LoadGameInstance = Cast<UBCSaveGame>(UGameplayStatics::CreateSaveGameObject(UBCSaveGame::StaticClass()));
//	LoadGameInstance = Cast<UBCSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
//	const FString PlayerNameToDisplay = LoadGameInstance->PlayerName;
//	
//	if (GEngine)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, PlayerNameToDisplay);
//	}
//}

