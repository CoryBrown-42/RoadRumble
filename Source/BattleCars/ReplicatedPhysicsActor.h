// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "ReplicatedPhysicsActor.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class BATTLECARS_API AReplicatedPhysicsActor : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AReplicatedPhysicsActor();

	/*UFUNCTION(Server, Reliable, WithValidation)
	virtual void SimulatePhysics();
	*/
	//UPROPERTY()
	//UStaticMeshComponent* Mesh;

};
