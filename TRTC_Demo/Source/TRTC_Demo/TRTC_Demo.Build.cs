// Copyright Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class TRTC_Demo : ModuleRules
{
    // 加载生成userSig 的framework。该方案仅适用于调试Demo，正式上线前请将 UserSig 计算代码和密钥迁移到您的后台服务器上，以避免加密密钥泄露导致的流量盗用。
    private void loadDebugDll(ReadOnlyTargetRules Target)
    {
        string _DebugPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "debug"));
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PublicIncludePaths.Add(Path.Combine(_DebugPath, "include"));   
        }else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            // 添加插件的包含路径
            PublicIncludePaths.Add(Path.Combine(_DebugPath, "include"));
            PublicAdditionalLibraries.Add(Path.Combine(_DebugPath, "ios", "libTXLiteAVTestUserSig.a"));
        }else if(Target.Platform == UnrealTargetPlatform.Mac)
        {
            // 添加插件的包含路径
            PublicIncludePaths.Add(Path.Combine(_DebugPath, "include"));
            PublicFrameworks.Add(Path.Combine(_DebugPath, "Mac", "Release","TXLiteAVTestUserSig.framework"));
            
        }else if (Target.Platform == UnrealTargetPlatform.Win64)
        {
	        PublicIncludePaths.Add(Path.Combine(_DebugPath, "include"));
            PublicFrameworks.Add(Path.Combine(_DebugPath, "win64", "Release","TXLiteAVTestUserSig.dll"));
            PublicAdditionalLibraries.Add(Path.Combine(_DebugPath, "win64", "Release","TXLiteAVTestUserSig.lib"));
            RuntimeDependencies.Add("$(BinaryOutputDir)/TXLiteAVTestUserSig.dll", Path.Combine(_DebugPath, "win64", "Release", "TXLiteAVTestUserSig.dll"));
        }
    }

    // 加载编译TRTC SDK framework
    private void loadTRTCSDK(ReadOnlyTargetRules Target)
    {
        string _TRTCSDKPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "TRTCSDK"));
        bEnableUndefinedIdentifierWarnings = false;
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PublicIncludePaths.Add(Path.Combine(_TRTCSDKPath, "include/Android"));
            PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });
            
            AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", Path.Combine(ModuleDirectory, "TRTCSDK", "Android", "APL_armv7.xml")));
			
			// // select required architecture
			string Architecture = "armeabi-v7a";
			// string Architecture = "arm64-v8a";
			// //string Architecture = "x86";
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory,"TRTCSDK", "Android", Architecture, "libtraeimp-rtmp.so"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory,"TRTCSDK", "Android", Architecture, "libliteavsdk.so"));
        }else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            // 添加插件的包含路径
            PublicIncludePaths.Add(Path.Combine(_TRTCSDKPath, "include/iOS"));
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
            PublicAdditionalFrameworks.Add(new UEBuildFramework( "TXLiteAVSDK_TRTC",_TRTCSDKPath+"/ios/TXLiteAVSDK_TRTC.framework.zip", ""));
        }else if(Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicIncludePaths.Add(Path.Combine(_TRTCSDKPath, "include/Mac"));
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
        }else if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicIncludePaths.Add(Path.Combine(_TRTCSDKPath, "include/win64"));
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
	public TRTC_Demo(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
        // //添加系统Framework和库
        // PrivateDependencyModuleNames.AddRange(new string[] { "TencentPlugin" });
		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] {"UMG", "Slate", "SlateCore"});
        // 导入TRTC SDK
        loadTRTCSDK(Target);
        loadDebugDll(Target);
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
   
}
