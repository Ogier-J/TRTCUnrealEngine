// Fill out your copyright notice in the Description page of Project Settings.


#include "BtnTRTCUserWidget.h"
#include "Engine/Engine.h"
#include <string>
#include "DebugDefs.h"
#ifdef __APPLE__

#endif

void UBtnTRTCUserWidget::handleTestButtonClick() {
    writeLblLog("start handleTestButtonClick");
    pTRTCCloud->startLocalAudio(trtc::TRTCAudioQualityDefault);
}
void UBtnTRTCUserWidget::OnStopLocalPreview_Click() {
    writeLblLog("start OnStopLocalPreview_Click");
    pTRTCCloud->stopLocalPreview();
    ResetBuffer(true);
    localRenderTargetTexture->UpdateTextureRegions(0, 1, localUpdateTextureRegion, localWidth * 4, (uint32)4, localBuffer);
}
void UBtnTRTCUserWidget::OnStartScreen_Click() {
    writeLblLog("start OnStartScreen_Click");
#if PLATFORM_WINDOWS
    trtc::ITRTCScreenCaptureSourceList*  wnd_info_list = pTRTCCloud->getScreenCaptureSources(SIZE{ 20, 20 }, SIZE{ 20,20 });
    if (wnd_info_list->getCount() > 0 )
    {
        trtc::TRTCScreenCaptureSourceInfo source_info = wnd_info_list->getSourceInfo(0);
        RECT capture_rect = { 0, 0, 0, 0 };
        trtc::TRTCScreenCaptureProperty capture_property;
        pTRTCCloud->selectScreenCaptureTarget(source_info, capture_rect, capture_property);
        trtc::TRTCVideoEncParam param;
        pTRTCCloud->startScreenCapture(nullptr, trtc::TRTCVideoStreamTypeSub, &param);
        writeLblLog("end startScreenCapture");
    }
#endif
}
void UBtnTRTCUserWidget::OnExitRoom_Click() {
    writeLblLog("start OnExitRoom_Click");
    pTRTCCloud->exitRoom();
}
void UBtnTRTCUserWidget::OnStopScreen_Click() {
    writeLblLog("start OnStopScreen_Click");
#if PLATFORM_WINDOWS
    pTRTCCloud->stopScreenCapture();
    ResetBuffer(false);
#endif
}
void UBtnTRTCUserWidget::OnStartLocalPreview_Click() {
    writeLblLog("start OnStartLocalPreview_Click");
#if PLATFORM_IOS || PLATFORM_ANDROID
    pTRTCCloud->startLocalPreview(true, nullptr);
#else
    pTRTCCloud->startLocalPreview(nullptr);
#endif
    #if PLATFORM_ANDROID
        pTRTCCloud->setLocalVideoRenderCallback(trtc::TRTCVideoPixelFormat_RGBA32, trtc::TRTCVideoBufferType_Buffer, this);
    #else
        pTRTCCloud->setLocalVideoRenderCallback(trtc::TRTCVideoPixelFormat_BGRA32, trtc::TRTCVideoBufferType_Buffer, this);
    #endif
    writeLblLog("end OnStartLocalPreview_Click");
}
void UBtnTRTCUserWidget::OnEnterRoom_Click() {
    // 请务必加上
    pTRTCCloud->callExperimentalAPI("{\"api\": \"setFramework\", \"params\": {\"framework\": 9}}");
    writeLblLog("start OnEnterRoom_Click roomid");
    // 构造进房参数
    trtc::TRTCParams params;
    params.role = trtc::TRTCRoleAnchor;
    params.userDefineRecordId = "";
    FString cUserId = txtUserId->GetText().ToString();
    params.userId = TCHAR_TO_ANSI(*cUserId);
    params.roomId = 2869;

#if PLATFORM_MAC || PLATFORM_IOS || PLATFORM_WINDOWS
   params.sdkAppId = SDKAppID;
   params.userSig = GenerateTestUserSig().genTestUserSig(params.userId, SDKAppID, SECRETKEY);
   pTRTCCloud->enterRoom(params, trtc::TRTCAppSceneVideoCall);
#else
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv()) {
        jmethodID GetPackageNameMethodID = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "genTestUserSig", "(ILjava/lang/String;Ljava/lang/String;)Ljava/lang/String;", false);
        jstring jsUserId = Env->NewStringUTF(testUserId);
        jstring jsKey = Env->NewStringUTF(SECRETKEY);
        jstring JstringResult = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis,GetPackageNameMethodID, SDKAppID, jsUserId, jsKey);
        FString FinalResult = FJavaHelper::FStringFromLocalRef(Env, JstringResult);
        auto twoHundredAnsi = StringCast<ANSICHAR>(*FinalResult);
        const char* userSig = twoHundredAnsi.Get();

        params.sdkAppId = SDKAppID;
        params.userSig = userSig;
        writeLblLog("=====enterRoom");
        // 进房，因为生成userSig的关系，enterRoom必须放在这个条件语句里
        pTRTCCloud->enterRoom(params, trtc::TRTCAppSceneVideoCall);
    }
    pTRTCCloud->startLocalAudio(trtc::TRTCAudioQualityDefault);
    writeLblLog("end OnEnterRoom_Click");
#endif
}

void UBtnTRTCUserWidget::onRenderVideoFrame(const char *userId, trtc::TRTCVideoStreamType streamType, trtc::TRTCVideoFrame *videoFrame) {
    int frameLength = videoFrame->length;
    if (localPreviewImage && remoteImage && frameLength > 1) {
        // 获取到 帧数据
        bool isLoaclUser =((strcmp(testUserId, userId) == 0 || nullptr == userId || strlen(userId)==0) && streamType ==  trtc::TRTCVideoStreamTypeBig)? true:false;
        UpdateBuffer(videoFrame->data,videoFrame->width,videoFrame->height,frameLength,isLoaclUser);
    }
}

void UBtnTRTCUserWidget::UpdateBuffer(
	char* RGBBuffer,
	uint32_t NewWidth,
	uint32_t NewHeight,
	uint32_t NewSize,
    bool isLocal)
{
    if (isLocal)
    {
        FScopeLock lock(&localMutex);
        if (!RGBBuffer)
        {
            return;
        }
        if(NewHeight != localHeight)
        {
            UE_LOG(LogTemp, Warning, TEXT("NewHeight != localHeight ,localHeight=%d, NewHeight=%d"),
            localHeight,NewHeight);
        }
        if(!localRefresh)
        {
            // 本地第一帧
            UE_LOG(LogTemp, Warning, TEXT("localRefresh==false; NewSize=%d ,NewWidth=%d, NewHeight=%d"),
            NewSize,NewWidth,NewHeight);
            localRefresh = true;
            AsyncTask(ENamedThreads::GameThread, [=]() {
                localWidth = NewWidth;
                localHeight = NewHeight;
                #if PLATFORM_ANDROID
                    localRenderTargetTexture = UTexture2D::CreateTransient(localWidth, localHeight, PF_R8G8B8A8);
                #else
                    localRenderTargetTexture = UTexture2D::CreateTransient(localWidth, localHeight);
                #endif
                localRenderTargetTexture->UpdateResource();
                localBufferSize= localWidth * localHeight * 4;
                localBuffer = new uint8[localBufferSize];
                for (uint32 i = 0; i < localWidth * localHeight; ++i)
                {
                    localBuffer[i * 4 + 0] = 0x32;
                    localBuffer[i * 4 + 1] = 0x32;
                    localBuffer[i * 4 + 2] = 0x32;
                    localBuffer[i * 4 + 3] = 0xFF;
                }
                localUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, localWidth, localHeight);
                localRenderTargetTexture->UpdateTextureRegions(0, 1, localUpdateTextureRegion, localWidth * 4, (uint32)4, localBuffer);
                localBrush.SetResourceObject(localRenderTargetTexture);
                localPreviewImage->SetBrush(localBrush);
            });
            return;
        }
        if (localBufferSize== NewSize)
        {
            localWidth = NewWidth;
            localHeight = NewHeight;
            std::copy(RGBBuffer, RGBBuffer + NewSize, localBuffer);
        }
        else
        {
            delete[] localBuffer;
            localBufferSize= NewSize;
            localWidth = NewWidth;
            localHeight = NewHeight;
            localBuffer = new uint8[localBufferSize];
            std::copy(RGBBuffer, RGBBuffer + NewSize, localBuffer);
        }
        localRefresh = true;
    }
	else
    {
        FScopeLock lock(&remoteMutex);
        if (!RGBBuffer)
        {
            return;
        }
        if(NewHeight != remoteHeight)
        {
            UE_LOG(LogTemp, Warning, TEXT("NewHeight != remoteHeight ,remoteHeight=%d, NewHeight=%d"),
            remoteHeight,NewHeight);
        }
        if(!remoteRefresh)
        {
            // 远端第一帧
            UE_LOG(LogTemp, Warning, TEXT("remoteRefresh==false; NewSize=%d ,NewWidth=%d, NewHeight=%d"),
            NewSize,NewWidth,NewHeight);
            remoteRefresh = true;
            AsyncTask(ENamedThreads::GameThread, [=]() {
                remoteWidth = NewWidth;
                remoteHeight = NewHeight;
                #if PLATFORM_ANDROID
                    remoteRenderTargetTexture = UTexture2D::CreateTransient(localWidth, localHeight, PF_R8G8B8A8);
                #else
                    remoteRenderTargetTexture = UTexture2D::CreateTransient(localWidth, localHeight);
                #endif
                remoteRenderTargetTexture->UpdateResource();
                remoteBufferSize= remoteWidth * remoteHeight * 4;
                remoteBuffer = new uint8[remoteBufferSize];
                for (uint32 i = 0; i < remoteWidth * remoteHeight; ++i)
                {
                    remoteBuffer[i * 4 + 0] = 0x32;
                    remoteBuffer[i * 4 + 1] = 0x32;
                    remoteBuffer[i * 4 + 2] = 0x32;
                    remoteBuffer[i * 4 + 3] = 0xFF;
                }
                remoteUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, remoteWidth, remoteHeight);
                remoteRenderTargetTexture->UpdateTextureRegions(0, 1, remoteUpdateTextureRegion, remoteWidth * 4, (uint32)4,remoteBuffer);
                remoteBrush.SetResourceObject(remoteRenderTargetTexture);
                remoteImage->SetBrush(remoteBrush);
            });
            return;
        }
        if (remoteBufferSize== NewSize)
        {
            remoteWidth = NewWidth;
            remoteHeight = NewHeight;
            std::copy(RGBBuffer, RGBBuffer + NewSize, remoteBuffer);
        }
        else
        {
            delete[] remoteBuffer;
            remoteBufferSize= NewSize;
            remoteWidth = NewWidth;
            remoteHeight = NewHeight;
            remoteBuffer = new uint8[remoteBufferSize];
            std::copy(RGBBuffer, RGBBuffer + NewSize, remoteBuffer);
        }
        remoteRefresh = true;
    }
}

void UBtnTRTCUserWidget::ResetBuffer(bool isLocal)
{
    if (isLocal)
    {
        for (uint32 i = 0; i < localWidth * localHeight; ++i)
        {
            localBuffer[i * 4 + 0] = 0x32;
            localBuffer[i * 4 + 1] = 0x32;
            localBuffer[i * 4 + 2] = 0x32;
            localBuffer[i * 4 + 3] = 0xFF;
        }
        localRefresh = false;
    }
    else
    {
        for (uint32 i = 0; i < remoteWidth * remoteHeight; ++i)
        {
            remoteBuffer[i * 4 + 0] = 0x32;
            remoteBuffer[i * 4 + 1] = 0x32;
            remoteBuffer[i * 4 + 2] = 0x32;
            remoteBuffer[i * 4 + 3] = 0xFF;
        }
        remoteRefresh = false;
    }
}

void UBtnTRTCUserWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime) {
    Super::NativeTick(MyGeometry, DeltaTime);
    // 更新本地视频画面
    if(localRefresh && localRenderTargetTexture && localBuffer && localPreviewImage && localBufferSize > 4) {
        FScopeLock lock(&localMutex);
        if (localRenderTargetTexture->GetSizeX() != localWidth ||
        localRenderTargetTexture->GetSizeY() != localHeight)
        {
            UE_LOG(LogTemp, Warning, TEXT("localBufferSize=%d ,localWidth=%d, localHeight=%d localRenderTargetTexture->GetSizeX =%d , localRenderTargetTexture->GetSizeY =%d"),
            localBufferSize,localWidth,localHeight,localRenderTargetTexture->GetSizeX(),localRenderTargetTexture->GetSizeY());
            auto NewUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, localWidth, localHeight);
            // PF_R8G8B8A8
            // macos PF_B8G8R8A8 --> TRTCVideoPixelFormat_BGRA32 验证通过
            #if PLATFORM_ANDROID
                auto NewRenderTargetTexture = UTexture2D::CreateTransient(localWidth, localHeight, PF_R8G8B8A8);
            #else
                auto NewRenderTargetTexture = UTexture2D::CreateTransient(localWidth, localHeight);
            #endif
            NewRenderTargetTexture->UpdateResource();
            NewRenderTargetTexture->UpdateTextureRegions(0, 1, NewUpdateTextureRegion, localWidth * 4, (uint32)4, localBuffer);
            localBrush.SetResourceObject(NewRenderTargetTexture);
            localPreviewImage->SetBrush(localBrush);
            FUpdateTextureRegion2D* TmpUpdateTextureRegion = localUpdateTextureRegion;
            localRenderTargetTexture = NewRenderTargetTexture;
            localUpdateTextureRegion = NewUpdateTextureRegion;
            delete TmpUpdateTextureRegion;
            return;
        }
        localRenderTargetTexture->UpdateTextureRegions(0, 1, localUpdateTextureRegion, localWidth * 4, (uint32)4, localBuffer);
    }
    // 更新远端用户画面
    if(remoteRefresh && remoteRenderTargetTexture && remoteBuffer && remoteImage && remoteBufferSize > 4) {
        FScopeLock lock(&remoteMutex);
        if (remoteRenderTargetTexture->GetSizeX() != remoteWidth ||
        remoteRenderTargetTexture->GetSizeY() != remoteHeight)
        {
            UE_LOG(LogTemp, Warning, TEXT("remoteBufferSize=%d, remoteWidth=%d, remoteHeight=%d  remoteRenderTargetTexture->GetSizeX =%d , remoteRenderTargetTexture->GetSizeY =%d" ),
            remoteBufferSize,remoteWidth,remoteHeight,remoteRenderTargetTexture->GetSizeX(),remoteRenderTargetTexture->GetSizeY());
            auto NewRMUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0,remoteWidth,remoteHeight);
            #if PLATFORM_ANDROID
                auto NewRMRenderTargetTexture = UTexture2D::CreateTransient(remoteWidth,remoteHeight, PF_R8G8B8A8);
            #else
                auto NewRMRenderTargetTexture = UTexture2D::CreateTransient(remoteWidth,remoteHeight);
            #endif
            NewRMRenderTargetTexture->UpdateResource();
            NewRMRenderTargetTexture->UpdateTextureRegions(0, 1, NewRMUpdateTextureRegion,remoteWidth * 4, (uint32)4,remoteBuffer);
            remoteBrush.SetResourceObject(NewRMRenderTargetTexture);
            remoteImage->SetBrush(remoteBrush);
            FUpdateTextureRegion2D* TmpRMUpdateTextureRegion = remoteUpdateTextureRegion;
            remoteRenderTargetTexture = NewRMRenderTargetTexture;
            remoteUpdateTextureRegion = NewRMUpdateTextureRegion;
            delete TmpRMUpdateTextureRegion;
            return;
        }
        remoteRenderTargetTexture->UpdateTextureRegions(0, 1,remoteUpdateTextureRegion,remoteWidth * 4, (uint32)4,remoteBuffer);
    }

}
void UBtnTRTCUserWidget::NativeConstruct() {
    Super::NativeConstruct();
    #if PLATFORM_ANDROID
        if (JNIEnv* Env = FAndroidApplication::GetJavaEnv()) {
            void* activity = (void*) FAndroidApplication::GetGameActivityThis();
            pTRTCCloud = getTRTCShareInstance(activity);
        }
    #else
        pTRTCCloud = getTRTCShareInstance();
    #endif
    pTRTCCloud->addCallback(this);
    std::string version = pTRTCCloud->getSDKVersion();
    btnEnterroom->OnClicked.AddDynamic(this, &UBtnTRTCUserWidget::OnEnterRoom_Click);
    btnTrtcTest->OnClicked.AddDynamic(this, &UBtnTRTCUserWidget::handleTestButtonClick);
    btnLocalPreview->OnClicked.AddDynamic(this, &UBtnTRTCUserWidget::OnStartLocalPreview_Click);
    btnStopPreview->OnClicked.AddDynamic(this, &UBtnTRTCUserWidget::OnStopLocalPreview_Click);
    BtnScreenCapture->OnClicked.AddDynamic(this, &UBtnTRTCUserWidget::OnStartScreen_Click);
    BtnExitRoom->OnClicked.AddDynamic(this, &UBtnTRTCUserWidget::OnExitRoom_Click);
    BtnStopScreen->OnClicked.AddDynamic(this, &UBtnTRTCUserWidget::OnStopScreen_Click);
    std::string stdStrTemp1(testStrRoomId);
    FString tempRoomId = stdStrTemp1.c_str();
    txtRoomID->SetText(FText::FromString(tempRoomId));
    std::string stdStrTemp2(testUserId);
    FString tempText = stdStrTemp2.c_str();
    txtUserId->SetText(FText::FromString(tempText));
    writeLblLog(version.c_str());
}

void UBtnTRTCUserWidget::NativeDestruct() {
    Super::NativeDestruct();
    if (pTRTCCloud != nullptr) {
        pTRTCCloud->exitRoom();
        pTRTCCloud->removeCallback(this);
        pTRTCCloud->destroyTRTCShareInstance();
        pTRTCCloud = nullptr;
    }
    delete[] localBuffer;
	delete localUpdateTextureRegion;
    delete[] remoteBuffer;
	delete remoteUpdateTextureRegion;
}
void UBtnTRTCUserWidget::writeLblLog(const char * logStr) {
    std::string stdStrLog(logStr);
    FString log = stdStrLog.c_str();
    UE_LOG(LogTemp,Log,TEXT("==> %s"), *log);
    if (txtLog != nullptr) {
        AsyncTask(ENamedThreads::GameThread, [=]() {
            txtLog->SetText(FText::FromString(log));
        });
        
    }else{
        AsyncTask(ENamedThreads::GameThread, [=]()
        {
            UE_LOG(LogTemp,Warning, TEXT("txtLog not find"));
        });
    }
}
void UBtnTRTCUserWidget::writeCallbackLog(const char * logStr) {
    std::string stdStrLog(logStr);
    FString log = stdStrLog.c_str();
    UE_LOG(LogTemp,Log, TEXT("<== %s"),*log);
    if (txtcallBack != nullptr) {
        AsyncTask(ENamedThreads::GameThread, [=]() {
            txtcallBack->SetText(FText::FromString(log));
        });
    }else{
        AsyncTask(ENamedThreads::GameThread, [=]()
        {
            UE_LOG(LogTemp,Warning, TEXT("txtcallBack not find"));
        });
    }
}
void  UBtnTRTCUserWidget::onUserVideoAvailable(const char *userId, bool available) {
    writeCallbackLog("onUserVideoAvailable");
    writeCallbackLog(userId);
    if (available) {
        pTRTCCloud->startRemoteView(userId, trtc::TRTCVideoStreamTypeBig, nullptr);
        pTRTCCloud->muteRemoteVideoStream(userId, false);
        #if PLATFORM_ANDROID
            pTRTCCloud->setRemoteVideoRenderCallback(userId,trtc::TRTCVideoPixelFormat_RGBA32,trtc::TRTCVideoBufferType_Buffer, this);
        #else
            pTRTCCloud->setRemoteVideoRenderCallback(userId,trtc::TRTCVideoPixelFormat_BGRA32,trtc::TRTCVideoBufferType_Buffer, this);
        #endif
    }else{
        pTRTCCloud->muteRemoteVideoStream(userId, true);
        ResetBuffer(false);
        remoteRenderTargetTexture->UpdateTextureRegions(0, 1, remoteUpdateTextureRegion, remoteWidth * 4, (uint32)4,remoteBuffer);
    }
}
void  UBtnTRTCUserWidget::onUserSubStreamAvailable(const char *userId, bool available) {
    writeCallbackLog("onUserSubStreamAvailable");
    writeCallbackLog(userId);
    if (available) {
        pTRTCCloud->startRemoteView(userId, trtc::TRTCVideoStreamTypeSub, nullptr);
        pTRTCCloud->muteRemoteVideoStream(userId, trtc::TRTCVideoStreamTypeSub, false);
        #if PLATFORM_ANDROID
            pTRTCCloud->setRemoteVideoRenderCallback(userId,trtc::TRTCVideoPixelFormat_RGBA32,trtc::TRTCVideoBufferType_Buffer, this);
        #else
            pTRTCCloud->setRemoteVideoRenderCallback(userId,trtc::TRTCVideoPixelFormat_BGRA32,trtc::TRTCVideoBufferType_Buffer, this);
        #endif
    }else{
        pTRTCCloud->muteRemoteVideoStream(userId, trtc::TRTCVideoStreamTypeSub, true);
        ResetBuffer(false);
        remoteRenderTargetTexture->UpdateTextureRegions(0, 1, remoteUpdateTextureRegion, remoteWidth * 4, (uint32)4,remoteBuffer);
    }
}
void UBtnTRTCUserWidget::onWarning(TXLiteAVWarning warningCode, const char *warningMsg, void *extraInfo) {
    writeCallbackLog(warningMsg);
}
void UBtnTRTCUserWidget::onError(TXLiteAVError errCode, const char *errMsg, void *extraInfo) {
    writeCallbackLog(errMsg);
}
void UBtnTRTCUserWidget::onEnterRoom(int result) {
    writeCallbackLog("onEnterRoom");
    UE_LOG(LogTemp,Log,TEXT("<== %d"), result);
}
void UBtnTRTCUserWidget::onExitRoom(int reason) {
    writeCallbackLog("onExitRoom");
    UE_LOG(LogTemp,Log,TEXT("<== %d"), reason);
}
