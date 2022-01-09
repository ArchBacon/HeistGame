// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HeistGame : ModuleRules
{
	public HeistGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule" });
        
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore"});
    }
}
