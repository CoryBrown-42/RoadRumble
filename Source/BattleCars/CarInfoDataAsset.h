// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CarInfoDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class BATTLECARS_API UCarInfoDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
		UPROPERTY(EditAnywhere)
		float Health = 100;
	
};
