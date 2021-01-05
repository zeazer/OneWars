// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class OneWarsEditorTarget : TargetRules
{
	public OneWarsEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		//bUseUnityBuild = false;
		//bUsePCHFiles = false;
		ExtraModuleNames.Add("OneWars");
	}
}
