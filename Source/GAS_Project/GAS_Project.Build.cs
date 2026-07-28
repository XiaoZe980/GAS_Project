// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GAS_Project : ModuleRules
{
	public GAS_Project(ReadOnlyTargetRules Target) : base(Target)
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
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"GAS_Project",
			"GAS_Project/GAS",
			"GAS_Project/Variant_Platforming",
			"GAS_Project/Variant_Platforming/Animation",
			"GAS_Project/Variant_Combat",
			"GAS_Project/Variant_Combat/AI",
			"GAS_Project/Variant_Combat/Animation",
			"GAS_Project/Variant_Combat/Gameplay",
			"GAS_Project/Variant_Combat/Interfaces",
			"GAS_Project/Variant_Combat/UI",
			"GAS_Project/Variant_SideScrolling",
			"GAS_Project/Variant_SideScrolling/AI",
			"GAS_Project/Variant_SideScrolling/Gameplay",
			"GAS_Project/Variant_SideScrolling/Interfaces",
			"GAS_Project/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
