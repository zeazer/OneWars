// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OneWars : ModuleRules
{
	public OneWars(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		bEnforceIWYU = true;
		bEnableExceptions = true;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule" , "ProceduralMeshComponent", "UMG", "OnlineSubsystem", "OnlineSubsystemSteam"});
    }
}
