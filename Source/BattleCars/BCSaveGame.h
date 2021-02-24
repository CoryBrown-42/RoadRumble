// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleCars.h"
#include "Item.h"
#include "GameFramework/SaveGame.h"
#include "BCSaveGame.generated.h"

/**
 * 
 */

namespace SaveGameVersion
{
	enum type
	{

		Initial,
		AddedInventory,
		AddedItemData,
		VersionPlusOne,
		LatestVersion = VersionPlusOne - 1

	};
 }
UCLASS(BlueprintType)
class BATTLECARS_API UBCSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UBCSaveGame()
	{
		SavedDataVersion = SaveGameVersion::LatestVersion;
	}

	//UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = SaveFile)
	//TMap<FPrimaryAssetId, ItemData> InventoryData;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = SaveFile)
	FString UserId;

	UPROPERTY(VisibleAnywhere, Category = SaveFile)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = SaveFile)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = SaveFile)
	uint32 UserIndex;


protected:

	UPROPERTY()
	TArray<FPrimaryAssetId> InventoryItems_DEPRECATED;

	UPROPERTY()
	int32 SavedDataVersion;

	virtual void Serialize(FArchive& Ar) override;
	
};
