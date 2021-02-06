// Copyright Epic Games, Inc. All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>
//#include "AudioCaptureComponent.h"
#include "BattleCarsPawn.generated.h"

//class UAudioCaptureComponent;
class UCameraComponent;
class USpringArmComponent;
class UTextRenderComponent;
class UInputComponent;

PRAGMA_DISABLE_DEPRECATION_WARNINGS

UCLASS(config=Game)
class ABattleCarsPawn : public AWheeledVehicle//, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Component that captures their audio */
	//UPROPERTY(Category = "Audio", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//UAudioCaptureComponent* AudioCapture;

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	/** SCene component for the In-Car view origin */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* InternalCameraBase;

	/** Camera component for the In-Car view */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* InternalCamera;

	/** Text component for the In-Car speed */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* InCarSpeed;

	/** Text component for the In-Car gear */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* InCarGear;


	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	//class UGASAbilitySystemComponent* AbilitySystemComponent;


	//UPROPERTY()
	//class UGASAttributeSet* Attributes;

	
public:
	ABattleCarsPawn();

	/** The current speed as a string eg 10 km/h */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
	FText SpeedDisplayString;

	/** The current gear as a string (R,N, 1,2 etc) */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
	FText GearDisplayString;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
	/** The color of the in-car gear text in forward gears */
	FColor	GearDisplayColor;

	/** The color of the in-car gear text when in reverse */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
	FColor	GearDisplayReverseColor;

	/** Are we using in-car camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly)
	bool bInCarCameraActive;

	/** Are we in reverse gear */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly)
	bool bInReverseGear;

	/** Initial offset of in-car camera */
	FVector InternalCameraOrigin;

	// Begin Pawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	//
	//virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/*GAS*/
	//virtual void InitializeAttributes();
	//virtual void GiveAbilities();
	//
	//virtual void PossessedBy(AController* NewController) override;
	//virtual void OnRep_PlayerState() override;
	//
	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	//TSubclassOf<class UGameplayEffect> DefaultAttributeEfffect;
	//
	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	//TArray<TSubclassOf<class UGASGameplayAbility>> DefaultAbilities;



	// End Pawn interface

	// Begin Actor interface
	virtual void Tick(float Delta) override;
protected:
	virtual void BeginPlay() override;

public:
	// End Actor interface

	/** Handle pressing forwards */
	void MoveForward(float Val);
	/** Setup the strings used on the HUD */
	void SetupInCarHUD();
	/** Update the physics material used by the vehicle mesh */
	void UpdatePhysicsMaterial();
	/** Handle pressing right */
	void MoveRight(float Val);
	/** Handle handbrake pressed */
	void OnHandbrakePressed();
	/** Handle handbrake released */
	void OnHandbrakeReleased();
	/** Switch between cameras */
	void OnToggleCamera();
	/** Handle reset VR device */
	void OnResetVR();
	/** Fire weapon **/
	void OnWeaponFire();
	/** Fire weapon **/
	void InGameMenu();

	void PushToTalk();

	static const FName LookUpBinding;
	static const FName LookRightBinding;

private:
	/** 
	 * 
	 * Activate In-Car camera. Enable camera and sets visibility of in-car HUD display
	 * @param	bState true will enable in car view and set visibility of various if its doesn't match new state
	 * @param	bForce true will force to always change state
	 * 
	 **/
	void EnableIncarView( const bool bState, const bool bForce = false );

	/** Update the gear and speed strings */
	void UpdateHUDStrings();

	/* Are we on a 'slippery' surface */
	bool bIsLowFriction;


public:
	/** Returns SpringArm subobject **/
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
	/** Returns InternalCamera subobject **/
	FORCEINLINE UCameraComponent* GetInternalCamera() const { return InternalCamera; }
	/** Returns InCarSpeed subobject **/
	FORCEINLINE UTextRenderComponent* GetInCarSpeed() const { return InCarSpeed; }
	/** Returns InCarGear subobject **/
	FORCEINLINE UTextRenderComponent* GetInCarGear() const { return InCarGear; }

};

PRAGMA_ENABLE_DEPRECATION_WARNINGS

