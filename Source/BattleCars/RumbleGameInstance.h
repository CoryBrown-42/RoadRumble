// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RumbleGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BATTLECARS_API URumbleGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	URumbleGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init();

	UFUNCTION(exec)
	void Host();

	UFUNCTION(exec)
	void Join(const FString& Address);

	UFUNCTION(exec)
	void Kill();

};
