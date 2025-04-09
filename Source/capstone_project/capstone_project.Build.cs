// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class capstone_project : ModuleRules
{
	public capstone_project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","AIModule", "UMG", "Niagara" });
	}
}
