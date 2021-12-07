// Copyright Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class TencentPlugin : ModuleRules
{
	public TencentPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		// if (Target.Platform == UnrealTargetPlatform.Android)
		// {
				
		// }else if (Target.Platform == UnrealTargetPlatform.IOS)
		// {
		// 	PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "include/iOS"));
		// }
		// else if(Target.Platform == UnrealTargetPlatform.Mac)
		// {
		// 	PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "include/Mac"));
		// }
		// else if (Target.Platform == UnrealTargetPlatform.Win64)
		// {
		// 	PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "include/win64"));
		// }
		PublicIncludePaths.AddRange(
			new string[] {}
		);
		PublicDependencyModuleNames.AddRange(new string[]
			{
				"Core",
				"TRTCSDK",
        "Projects"
			}
		);
		PrivateDependencyModuleNames.AddRange(new string[]
			{
				"Engine",
				"Slate",
				"SlateCore",
				"UMG"
			}
		);
		DynamicallyLoadedModuleNames.AddRange(
			new string[]{}
		);
		
	}
}
