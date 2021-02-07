// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BattleCars : ModuleRules
{
	public BattleCars(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "PhysXVehicles", "HeadMountedDisplay", "UMG", "OnlineSubsystem" });
		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks" });
		PublicDefinitions.Add("HMD_MODULE_INCLUDED=1");
	}
}
