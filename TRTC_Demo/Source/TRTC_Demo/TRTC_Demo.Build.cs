// Copyright Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class TRTC_Demo : ModuleRules
{
    private string ThirdPartyPath
    {
        get
        {
            return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../Source/TRTCSDK"));
        }
    }
	public TRTC_Demo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
        // //添加系统Framework和库
		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] {"UMG", "Slate", "SlateCore" });
        
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            
        }else if (Target.Platform == UnrealTargetPlatform.IOS) {
            
            PublicAdditionalFrameworks.Add(new Framework( "TXLiteAVSDK_TRTC", Path.Combine(ThirdPartyPath, "iOS", "TXLiteAVSDK_TRTC.framework.zip"), "", true) );
            PublicAdditionalFrameworks.Add(new Framework( "TXLiteAVSDK_ReplayKitExt", Path.Combine(ThirdPartyPath, "iOS", "TXLiteAVSDK_ReplayKitExt.framework.zip"), "", true) );
            PublicFrameworks.AddRange(
                        new string[] {
                            "CoreML",
                            "VideoToolbox",
                            "Accelerate",
                            "CFNetwork",
                            "OpenGLES",
                            "CoreTelephony"
                }
            );
        }else if(Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicFrameworks.Add(Path.Combine(ThirdPartyPath, "Mac", "Release", "TXLiteAVSDK_TRTC_Mac.framework"));
            
            PublicAdditionalLibraries.Add ("resolv");
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
                    "CoreAudio"
                });
        }
       
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
