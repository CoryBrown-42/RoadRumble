// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCarsAssetManager.h"
#include "Item.h"
#include "AbilitySystemGlobals.h"

const FPrimaryAssetType	UBattleCarsAssetManager::PowerUpType = TEXT("PowerUp");
const FPrimaryAssetType	UBattleCarsAssetManager::PickUpType = TEXT("PickUp");
const FPrimaryAssetType	UBattleCarsAssetManager::DroppableType = TEXT("Droppable");
const FPrimaryAssetType	UBattleCarsAssetManager::WeaponType = TEXT("Weapon");

UBattleCarsAssetManager& UBattleCarsAssetManager::Get()
{
	UBattleCarsAssetManager* This = Cast<UBattleCarsAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be BattleCarsAssetManager!"));
		return *NewObject<UBattleCarsAssetManager>(); // never calls this
	}
}
void UBattleCarsAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}


UItem* UBattleCarsAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	// This does a synchronous load and may hitch
	UItem* LoadedItem = Cast<UItem>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}