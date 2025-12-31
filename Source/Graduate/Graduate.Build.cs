// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Graduate : ModuleRules
{
	public Graduate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "NiagaraCore", "Paper2D",  "Slate", "SlateCore", "LevelSequence", "MovieScene", "NavigationSystem"});

		PrivateDependencyModuleNames.AddRange(new string[] { "NavigationSystem", "Niagara", "AIModule", "UMG"});
	}
}
