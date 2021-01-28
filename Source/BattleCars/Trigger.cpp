// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger.h"

#include "Components/BoxComponent.h"
#include "MovingPlatform.h"


// Sets default values
ATrigger::ATrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerVolume = CreateDefaultSubobject <UBoxComponent>(FName("TriggerVolume"));

	if (!ensure(TriggerVolume != nullptr)) return;

	RootComponent = TriggerVolume;
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ATrigger::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &ATrigger::OnOverlapEnd);

}

// Called when the game starts or when spawned
void ATrigger::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Begin Overlap Event
void ATrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Log that trigger has been activated by an overlapped AActor
	UE_LOG(LogTemp, Warning, TEXT("On"));

	// Loop through the platforms this trigger affects and call AddActiveTrigger
	for (AMovingPlatform* Platform : PlatformsToTrigger)
	{
		Platform->AddActiveTrigger();
	}
}

//End Overlap Event
void ATrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Log that trigger has been deactivated.
	UE_LOG(LogTemp, Warning, TEXT("Off"));

	// Loop through the platforms this trigger affects and call RemoveActiveTrigger
	for (AMovingPlatform* Platform : PlatformsToTrigger)
	{
		Platform->RemoveActiveTrigger();
	}
}

