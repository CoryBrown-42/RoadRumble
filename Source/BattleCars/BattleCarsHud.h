// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "BattleCarsHud.generated.h"


UCLASS(config = Game)
class ABattleCarsHud : public AHUD
{
	GENERATED_BODY()

public:
	ABattleCarsHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface
};
