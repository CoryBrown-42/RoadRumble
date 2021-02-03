// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * A Blocking mesh that will move to a point and block the players progression.
 */
UCLASS()
class BATTLECARS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AMovingPlatform();
	
	//The rate at which the blocker moves
	UPROPERTY(EditAnywhere)
	float Speed = 300;

	//The place that the platform moves.
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
	FVector TargetLocation;



	void AddActiveTrigger();
	void RemoveActiveTrigger();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


private:

	UPROPERTY(VisibleAnywhere, Category = "Volume")
	class UBoxComponent* TriggerVolume;

	FVector GlobalTargetLocation;
	FVector GlobalStartLocation;

	UPROPERTY(EditAnywhere)
	int ActiveTriggers = 1;

};
