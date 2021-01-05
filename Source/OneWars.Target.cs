// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class OneWarsTarget : TargetRules
{
	public OneWarsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		//bUseUnityBuild = false;
		//bUsePCHFiles = false;
		ExtraModuleNames.Add("OneWars");
	}
}
