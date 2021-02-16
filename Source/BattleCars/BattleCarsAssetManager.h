// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BattleCars.h"
#include "Engine/AssetManager.h"
#include "BattleCarsAssetManager.generated.h"

class UItem;

/**
 * 
 */
UCLASS()
class BATTLECARS_API UBattleCarsAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	UBattleCarsAssetManager(){}
	virtual void StartInitialLoading() override;

	static const FPrimaryAssetType PowerUpType;
	static const FPrimaryAssetType PickUpType;
	static const FPrimaryAssetType DroppableType;
	static const FPrimaryAssetType WeaponType;

	static UBattleCarsAssetManager& Get();

	UItem* ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);

	
};
