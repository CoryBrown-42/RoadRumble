// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "WeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class BATTLECARS_API UWeaponItem : public UItem
{
	GENERATED_BODY()

public:
	
	UWeaponItem()
	{
		ItemType = UBattleCarsAssetManager::WeaponType;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<AActor> WeaponActor;
	
};
