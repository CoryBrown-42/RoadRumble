// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BCSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class BATTLECARS_API UBCSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UBCSaveGame();

	UFUNCTION()
	void SaveGame();

	UFUNCTION()
	void LoadGame();
	
};
