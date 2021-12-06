本文主要介绍如何快速地将腾讯云 TRTC SDK（Unreal Engine）集成到您的项目中，只要按照如下步骤进行配置，就可以完成 SDK 的集成工作。

## 环境要求
* 建议Unreal Engine 4.27.1 及以上版本。
* 目前支持 iOS、Windows、MacOS平台，Android还在开发中
- 其中 iOS  端开发还需要：
  - Xcode 11.0及以上版本。
  - 请确保您的项目已设置有效的开发者签名。
- **Windows 开发：**
    - 操作系统：Windows 7 SP1 或更高的版本（基于 x86-64 的 64 位操作系统）。
    - 磁盘空间：除安装 IDE 和一些工具之外还应有至少 1.64 GB 的空间。
    - 安装 [Visual Studio 2019](https://visualstudio.microsoft.com/zh-hans/downloads/)。

## 集成 SDK
1. 下载 SDK 及配套的 [SDK 源码]()（暂时加入群号：764231117 获取）。
2. 解压后，把项目中的 `TRTCSDK` 文件夹拷贝到您项目中的 **Source** 目录下。
3. 编辑你项目中的 **[project_name].Build.cs**文件。添加下面函数
```
// 加载编译TRTC SDK framework
private void loadTRTCSDK(ReadOnlyTargetRules Target)
{
    string _TRTCSDKPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "TRTCSDK"));
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
```
4. 在**[project_name].Build.cs**文件调用该函数
![](https://imgcache.qq.com/operation/dianshi/other/TRTCSDK.82d81b0b8fe050772b3c8e02c4578b920515a580.jpg)
5. 到目前为止你已经集成了TRTC SDK。可在你的cpp 文件中使用TRTC了。`#include "ITRTCCloud.h"`
```
trtc::ITRTCCloud * pTRTCCloud = getTRTCShareInstance();
pTRTCCloud->addCallback(this);
std::string version = pTRTCCloud->getSDKVersion();
```
## 其他
[TRTC C++ API]()
