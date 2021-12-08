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
		string _TRTCSDKPath =  Path.GetFullPath(Path.Combine(ModuleDirectory, "../ThirdParty/TRTCSDK"));; // ""
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
		if (Target.Platform == UnrealTargetPlatform.Android)
		{
				
		}else if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			// PublicAdditionalLibraries.AddRange(new string[] {
			// 		"resolv",
			// 		"z",
			// 		"c++",
			// });
			// PublicFrameworks.AddRange(
			// 		new string[] {
			// 				"CoreML",
			// 				"VideoToolbox",
			// 				"Accelerate",
			// 				"CFNetwork",
			// 				"OpenGLES",
			// 				"AVFoundation",
			// 				"CoreTelephony"
			// 		}
			// );
			PublicAdditionalFrameworks.Add(new UEBuildFramework( "TXLiteAVSDK_TRTC",_TRTCSDKPath+"/ios/TXLiteAVSDK_TRTC.framework.zip", ""));
		}else if(Target.Platform == UnrealTargetPlatform.Mac)
		{
			PublicAdditionalLibraries.AddRange(new string[] {
				"resolv",
				"z",
				"c++",
				"bz2",
			});
			PublicFrameworks.Add(Path.Combine(_TRTCSDKPath, "Mac", "Release","TXLiteAVSDK_TRTC_Mac.framework"));
		}else if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicAdditionalLibraries.Add(Path.Combine(_TRTCSDKPath, "win64", "Release","liteav.lib"));
			PublicDelayLoadDLLs.Add(Path.Combine(_TRTCSDKPath, "win64", "Release", "liteav.dll"));
			PublicDelayLoadDLLs.Add(Path.Combine(_TRTCSDKPath, "win64", "Release", "LiteAvAudioHook.dll"));
			PublicDelayLoadDLLs.Add(Path.Combine(_TRTCSDKPath, "win64", "Release", "LiteAvAudioHookService.dll"));
			PublicDelayLoadDLLs.Add(Path.Combine(_TRTCSDKPath, "win64", "Release", "openh264.dll"));
			PublicDelayLoadDLLs.Add(Path.Combine(_TRTCSDKPath, "win64", "Release", "TRAE.dll"));

			RuntimeDependencies.Add("$(BinaryOutputDir)/liteav.dll", Path.Combine(_TRTCSDKPath, "win64", "Release", "liteav.dll"));
			RuntimeDependencies.Add("$(BinaryOutputDir)/LiteAvAudioHook.dll", Path.Combine(_TRTCSDKPath, "win64", "Release", "LiteAvAudioHook.dll"));
			RuntimeDependencies.Add("$(BinaryOutputDir)/LiteAvAudioHookService.dll", Path.Combine(_TRTCSDKPath, "win64", "Release", "LiteAvAudioHookService.dll"));
			RuntimeDependencies.Add("$(BinaryOutputDir)/openh264.dll", Path.Combine(_TRTCSDKPath, "win64", "Release", "openh264.dll"));
			RuntimeDependencies.Add("$(BinaryOutputDir)/TRAE.dll", Path.Combine(_TRTCSDKPath, "win64", "Release", "TRAE.dll"));
		}
	}
}
