// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplicatedPhysicsActor.h"
#include "Components/PrimitiveComponent.h"

AReplicatedPhysicsActor::AReplicatedPhysicsActor()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
	bReplicates = true;

	///UStaticMeshComponent* Mesh;
	//Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	
	///static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/VehicleVarietyPack/Skeletons/SK_Pickup"));
	
}

//void AReplicatedPhysicsActor::SimulatePhysics()
//{
//	Mesh->SetSimulatePhysics(true);
//}