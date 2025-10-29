// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Faro : ModuleRules
{
	public Faro(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Faro",
			"Faro/Variant_Platforming",
			"Faro/Variant_Platforming/Animation",
			"Faro/Variant_Combat",
			"Faro/Variant_Combat/AI",
			"Faro/Variant_Combat/Animation",
			"Faro/Variant_Combat/Gameplay",
			"Faro/Variant_Combat/Interfaces",
			"Faro/Variant_Combat/UI",
			"Faro/Variant_SideScrolling",
			"Faro/Variant_SideScrolling/AI",
			"Faro/Variant_SideScrolling/Gameplay",
			"Faro/Variant_SideScrolling/Interfaces",
			"Faro/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
