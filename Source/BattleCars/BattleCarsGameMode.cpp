// Copyright Epic Games, Inc. All Rights Reserved.

#include "BattleCarsGameMode.h"
#include "BattleCarsPawn.h"
#include "BattleCarsHud.h"

ABattleCarsGameMode::ABattleCarsGameMode()
{
	DefaultPawnClass = ABattleCarsPawn::StaticClass();
}
