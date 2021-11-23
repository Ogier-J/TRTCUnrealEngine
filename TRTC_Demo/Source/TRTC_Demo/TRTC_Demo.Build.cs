// Copyright Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class TRTC_Demo : ModuleRules
{
    private string _TRTCSDKPath
    {
        get
        {
            return Path.GetFullPath(Path.Combine(ModuleDirectory, "TRTCSDK"));
        }
    }
    private string _BasePath
    {
        get
        {
            return Path.GetFullPath(Path.Combine(ModuleDirectory, "base"));
        }
    }
	public TRTC_Demo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
        // //添加系统Framework和库
		PrivateDependencyModuleNames.AddRange(new string[] {  });
		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] {"UMG", "Slate", "SlateCore"});
        // 添加插件的包含路径
        PublicIncludePaths.Add(Path.Combine(_TRTCSDKPath, "include"));
        PublicIncludePaths.Add(Path.Combine(_BasePath, "include"));
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            
        }else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PublicAdditionalLibraries.AddRange(new string[] {
                "resolv",
                "z",
                "c++",
            });
            PublicFrameworks.AddRange(
                new string[] {
                    "CoreML",
                    "VideoToolbox",
                    "Accelerate",
                    "CFNetwork",
                    "OpenGLES",
                    "AVFoundation",
                    "CoreTelephony"
                }
            );
            // PublicAdditionalFrameworks.Add(new Framework( "TXLiteAVSDK_TRTC",Path.Combine(_TRTCSDKPath, "iOS","TXLiteAVSDK_TRTC.framework.zip"), "", true));
            PublicAdditionalFrameworks.Add(new UEBuildFramework( "TXLiteAVSDK_TRTC",_TRTCSDKPath+"/ios/TXLiteAVSDK_TRTC.framework.zip", ""));
        }else if(Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicAdditionalLibraries.AddRange(new string[] {
                "resolv",
                "z",
                "c++",
                "bz2",
            });
            PublicFrameworks.AddRange(
                new string[] {
                    "AppKit",
                    "IOKit",
                    "CoreVideo",
                    "CFNetwork",
                    "OpenGl",
                    "CoreGraphics",
                    "Accelerate",
                    "CoreFoundation",
                    "SystemConfiguration",
                    "AudioToolbox",
                    "VideoToolbox",
                    "CoreTelephony",
                    "CoreWLAN",
                    "AVFoundation",
                    "CoreMedia",
                    "CoreAudio",
                    "AudioUnit",
                    "Accelerate",
                });
            PublicFrameworks.Add(Path.Combine(_TRTCSDKPath, "Mac", "Release","TXLiteAVSDK_TRTC_Mac.framework"));
            // 暂时注释
            // PublicFrameworks.Add(Path.Combine(_BasePath, "Mac", "Release","libTXLiteAVTestUserSig.a"));
            
        }else if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            
        }
        
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
