// Copyright Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class TencentPlugin : ModuleRules
{
	private string _TRTCSDKPath
	{
			get
			{
					return Path.GetFullPath(Path.Combine(ModuleDirectory, "TRTCSDK"));
			}
	}
	// private string _DebugPath
	// {
	// 		get
	// 		{
	// 				return Path.GetFullPath(Path.Combine(ModuleDirectory, "debug"));
	// 		}
	// }
	public TencentPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
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
