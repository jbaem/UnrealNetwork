// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealNetwork : ModuleRules
{
	public UnrealNetwork(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"SlateCore",
            "Niagara",
        });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"UnrealNetwork",
			"UnrealNetwork/Variant_Platforming",
			"UnrealNetwork/Variant_Platforming/Animation",
			"UnrealNetwork/Variant_Combat",
			"UnrealNetwork/Variant_Combat/AI",
			"UnrealNetwork/Variant_Combat/Animation",
			"UnrealNetwork/Variant_Combat/Gameplay",
			"UnrealNetwork/Variant_Combat/Interfaces",
			"UnrealNetwork/Variant_Combat/UI",
			"UnrealNetwork/Variant_SideScrolling",
			"UnrealNetwork/Variant_SideScrolling/AI",
			"UnrealNetwork/Variant_SideScrolling/Gameplay",
			"UnrealNetwork/Variant_SideScrolling/Interfaces",
			"UnrealNetwork/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
