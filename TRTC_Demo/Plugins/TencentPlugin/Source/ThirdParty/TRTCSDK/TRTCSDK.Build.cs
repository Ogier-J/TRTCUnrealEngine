
using System.IO;
using UnrealBuildTool;

public class TRTCSDK : ModuleRules
{
  // 加载编译TRTC SDK framework
    private void loadTRTCSDK(ReadOnlyTargetRules Target)
    {
      string _TRTCSDKPath = ModuleDirectory;
      bEnableUndefinedIdentifierWarnings = false;
      if (Target.Platform == UnrealTargetPlatform.Android)
      {
          
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
          }
        );
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
	public TRTCSDK(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
  }

}
