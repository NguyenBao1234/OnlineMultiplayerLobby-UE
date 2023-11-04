// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class M_GameCPP : ModuleRules
{
	public M_GameCPP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "OnlineSubsystem", "OnlineSubsystemSteam" });
    }
}
