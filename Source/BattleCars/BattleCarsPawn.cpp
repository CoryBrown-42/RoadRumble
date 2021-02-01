// Copyright Epic Games, Inc. All Rights Reserved.

#include "BattleCarsPawn.h"
#include "BattleCarsWheelFront.h"
#include "BattleCarsWheelRear.h"
#include "BattleCarsHud.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextRenderComponent.h"
#include "Materials/Material.h"
#include "GameFramework/Controller.h"
//#include "GASAbilitySystemComponent.h"
//#include "GASAttributeSet.h"
//#include "GASGameplayAbility.h"
//#include <GameplayEffectTypes.h>

#ifndef HMD_MODULE_INCLUDED
#define HMD_MODULE_INCLUDED 0
#endif

// Needed for VR Headset
#if HMD_MODULE_INCLUDED
#include "IXRTrackingSystem.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#endif // HMD_MODULE_INCLUDED

const FName ABattleCarsPawn::LookUpBinding("LookUp");
const FName ABattleCarsPawn::LookRightBinding("LookRight");

#define LOCTEXT_NAMESPACE "VehiclePawn"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

ABattleCarsPawn::ABattleCarsPawn()
{
	// Car mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/Vehicle/Sedan/Sedan_SkelMesh.Sedan_SkelMesh"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);

	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Vehicle/Sedan/Sedan_AnimBP"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	
	// Simulation
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(Vehicle4W->WheelSetups.Num() == 4);

	/*Front Wheels*/
	Vehicle4W->WheelSetups[0].WheelClass = UBattleCarsWheelFront::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("Wheel_Front_Left");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = UBattleCarsWheelFront::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("Wheel_Front_Right");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	/*Rear Wheels*/
	Vehicle4W->WheelSetups[2].WheelClass = UBattleCarsWheelRear::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("Wheel_Rear_Left");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = UBattleCarsWheelRear::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("Wheel_Rear_Right");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 200.f);
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 800.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	// Create In-Car camera component 
	InternalCameraOrigin = FVector(0.0f, -40.0f, 120.0f);

	InternalCameraBase = CreateDefaultSubobject<USceneComponent>(TEXT("InternalCameraBase"));
	InternalCameraBase->SetRelativeLocation(InternalCameraOrigin);
	InternalCameraBase->SetupAttachment(GetMesh());

	InternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InternalCamera"));
	InternalCamera->bUsePawnControlRotation = false;
	InternalCamera->FieldOfView = 90.f;
	InternalCamera->SetupAttachment(InternalCameraBase);

	//Setup TextRenderMaterial
	static ConstructorHelpers::FObjectFinder<UMaterial> TextMaterial(TEXT("Material'/Engine/EngineMaterials/AntiAliasedTextMaterialTranslucent.AntiAliasedTextMaterialTranslucent'"));
	
	UMaterialInterface* Material = TextMaterial.Object;

	// Create text render component for in car speed display
	InCarSpeed = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarSpeed"));
	InCarSpeed->SetTextMaterial(Material);
	InCarSpeed->SetRelativeLocation(FVector(70.0f, -75.0f, 99.0f));
	InCarSpeed->SetRelativeRotation(FRotator(18.0f, 180.0f, 0.0f));
	InCarSpeed->SetupAttachment(GetMesh());
	InCarSpeed->SetRelativeScale3D(FVector(1.0f, 0.4f, 0.4f));

	// Create text render component for in car gear display
	InCarGear = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
	InCarGear->SetTextMaterial(Material);
	InCarGear->SetRelativeLocation(FVector(66.0f, -9.0f, 95.0f));	
	InCarGear->SetRelativeRotation(FRotator(25.0f, 180.0f,0.0f));
	InCarGear->SetRelativeScale3D(FVector(1.0f, 0.4f, 0.4f));
	InCarGear->SetupAttachment(GetMesh());
	
	// Colors for the incar gear display. One for normal one for reverse
	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	// Colors for the in-car gear display. One for normal one for reverse
	GearDisplayReverseColor = FColor(255, 0, 0, 255);

	GearDisplayColor = FColor(255, 255, 255, 255);

	bInReverseGear = false;

	//AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>("AbilitySystemComponent");
	//AbilitySystemComponent->SetIsReplicated(true);
	//AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	//
	//Attributes = CreateDefaultSubobject<UGASAttributeSet>("Attributes");

}



void ABattleCarsPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//
	//// set up gameplay key bindings
	//check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABattleCarsPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABattleCarsPawn::MoveRight);
	PlayerInputComponent->BindAxis("LookUp");
	PlayerInputComponent->BindAxis("LookRight");

	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &ABattleCarsPawn::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &ABattleCarsPawn::OnHandbrakeReleased);
	PlayerInputComponent->BindAction("SwitchCamera", IE_Pressed, this, &ABattleCarsPawn::OnToggleCamera);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ABattleCarsPawn::OnWeaponFire);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ABattleCarsPawn::OnResetVR); 

	//if (AbilitySystemComponent && InputComponent)
	//{
	//	const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID", static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel));
	//	AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	//}
}

void ABattleCarsPawn::MoveForward(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);
	
}

void ABattleCarsPawn::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void ABattleCarsPawn::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void ABattleCarsPawn::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void ABattleCarsPawn::OnWeaponFire()
{
	//GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void ABattleCarsPawn::OnToggleCamera()
{
	EnableIncarView(!bInCarCameraActive);
}

void ABattleCarsPawn::EnableIncarView(const bool bState, const bool bForce)
{
	if ((bState != bInCarCameraActive) || ( bForce == true ))
	{
		bInCarCameraActive = bState;
		
		if (bState == true)
		{
			OnResetVR();
			Camera->Deactivate();
			InternalCamera->Activate();
		}
		else
		{
			InternalCamera->Deactivate();
			Camera->Activate();
		}
		
		InCarSpeed->SetVisibility(bInCarCameraActive);
		InCarGear->SetVisibility(bInCarCameraActive);
	}
}

//class UAbilitySystemComponent* ABattleCarsPawn::GetAbilitySystemComponent() const 
//{
//	return AbilitySystemComponent;
//}

//void ABattleCarsPawn::InitializeAttributes()
//{
//	if (AbilitySystemComponent && DefaultAttributeEfffect)
//	{
//		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
//		EffectContext.AddSourceObject(this);
//
//		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEfffect, 1, EffectContext);
//
//		if (SpecHandle.IsValid())
//		{
//			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
//		}
//
//	}
//}

//void ABattleCarsPawn::GiveAbilities()
//{
//	if (HasAuthority() && AbilitySystemComponent)
//	{
//		for (TSubclassOf<UGASGameplayAbility>& StartupAbility : DefaultAbilities)
//		{
//			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
//		}
//	}
//}

//void ABattleCarsPawn::PossessedBy(AController* NewController)
//{
//	Super::PossessedBy(NewController);
//
//	//Server GAS init
//	AbilitySystemComponent->InitAbilityActorInfo(this, this);
//	InitializeAttributes();
//	GiveAbilities();
//}
//
//void ABattleCarsPawn::OnRep_PlayerState()
//{
//	Super::OnRep_PlayerState();
//	AbilitySystemComponent->InitAbilityActorInfo(this, this);
//	InitializeAttributes();
//
//	if (AbilitySystemComponent && InputComponent)
//	{
//		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID", static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel));
//		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
//	}
//}

void ABattleCarsPawn::Tick(float Delta)
{
	Super::Tick(Delta);

	// Setup the flag to say we are in reverse gear
	bInReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;
	
	// Update the strings used in the HUD (incar and onscreen)
	UpdateHUDStrings();

	// Set the string in the incar HUD
	SetupInCarHUD();

	bool bHMDActive = false;
#if HMD_MODULE_INCLUDED
	if ((GEngine->XRSystem.IsValid() == true) && ((GEngine->XRSystem->IsHeadTrackingAllowed() == true) || (GEngine->IsStereoscopic3D() == true)))
	{
		bHMDActive = true;
	}
#endif // HMD_MODULE_INCLUDED
	if (bHMDActive == false)
	{
		if ( (InputComponent) && (bInCarCameraActive == true ))
		{
			FRotator HeadRotation = InternalCamera->GetRelativeRotation();
			HeadRotation.Pitch += InputComponent->GetAxisValue(LookUpBinding);
			HeadRotation.Yaw += InputComponent->GetAxisValue(LookRightBinding);
			InternalCamera->SetRelativeRotation(HeadRotation);
		}
	}
}

void ABattleCarsPawn::BeginPlay()
{
	Super::BeginPlay();

	bool bEnableInCar = false;
#if HMD_MODULE_INCLUDED
	bEnableInCar = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
#endif // HMD_MODULE_INCLUDED
	EnableIncarView(bEnableInCar,true);
}

void ABattleCarsPawn::OnResetVR()
{
#if HMD_MODULE_INCLUDED
	if (GEngine->XRSystem.IsValid())
	{
		GEngine->XRSystem->ResetOrientationAndPosition();
		InternalCamera->SetRelativeLocation(InternalCameraOrigin);
		GetController()->SetControlRotation(FRotator());
	}
#endif // HMD_MODULE_INCLUDED
}

void ABattleCarsPawn::UpdateHUDStrings()
{
	float KPH = FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) * 0.036f;
	int32 KPH_int = FMath::FloorToInt(KPH);

	// Using FText because this is display text that should be localizable
	SpeedDisplayString = FText::Format(LOCTEXT("SpeedFormat", "{0} km/h"), FText::AsNumber(KPH_int));
	
	if (bInReverseGear == true)
	{
		GearDisplayString = FText(LOCTEXT("ReverseGear", "R"));
	}
	else
	{
		int32 Gear = GetVehicleMovement()->GetCurrentGear();
		GearDisplayString = (Gear == 0) ? LOCTEXT("N", "N") : FText::AsNumber(Gear);
	}	
}

void ABattleCarsPawn::SetupInCarHUD()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if ((PlayerController != nullptr) && (InCarSpeed != nullptr) && (InCarGear != nullptr) )
	{
		// Setup the text render component strings
		InCarSpeed->SetText(SpeedDisplayString);
		InCarGear->SetText(GearDisplayString);
		
		if (bInReverseGear == false)
		{
			InCarGear->SetTextRenderColor(GearDisplayColor);
		}
		else
		{
			InCarGear->SetTextRenderColor(GearDisplayReverseColor);
		}
	}
}

#undef LOCTEXT_NAMESPACE

PRAGMA_ENABLE_DEPRECATION_WARNINGS
