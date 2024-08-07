// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class pj_AutomationTesting : ModuleRules
{
	public pj_AutomationTesting(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
