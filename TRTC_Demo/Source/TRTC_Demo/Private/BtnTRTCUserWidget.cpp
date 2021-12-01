// Fill out your copyright notice in the Description page of Project Settings.


#include "BtnTRTCUserWidget.h"
#include "Engine/Engine.h"
#include <string>
#include "DebugDefs.h"
#ifdef __APPLE__

#endif

void UBtnTRTCUserWidget::handleTestButtonClick() {
    writeLblLog("start handleTestButtonClick");
}
void UBtnTRTCUserWidget::OnStopLocalPreview_Click() {
    writeLblLog("start OnStopLocalPreview_Click");
    pTRTCCloud->stopLocalPreview();
    ResetBuffer(true);
}
void UBtnTRTCUserWidget::OnStartLocalPreview_Click() {
    writeLblLog("start OnStartLocalPreview_Click");
    
#if PLATFORM_IOS || PLATFORM_ANDROID
    pTRTCCloud->startLocalPreview(true, nullptr);
#else
    pTRTCCloud->startLocalPreview(nullptr);
#endif
    pTRTCCloud->setLocalVideoRenderCallback(trtc::TRTCVideoPixelFormat_BGRA32
, trtc::TRTCVideoBufferType_Buffer, this);
    writeLblLog("end OnStartLocalPreview_Click");
}
void UBtnTRTCUserWidget::OnEnterRoom_Click() {
    writeLblLog("start OnEnterRoom_Click roomid: 110");
    // 构造进房参数
    trtc::TRTCParams params;
    params.role = trtc::TRTCRoleAnchor;
    params.userDefineRecordId = "";
    params.userId = testUserId;
    params.roomId =  110;
    // 暂时只支持macos。
#if PLATFORM_MAC
   params.sdkAppId = SDKAppID;
   params.userSig = GenerateTestUserSig().genTestUserSig(testUserId, SDKAppID, SECRETKEY);
#else
   params.sdkAppId = testSDKAppID;
   params.userSig = testUserSig;
#endif
    // 进房
    pTRTCCloud->enterRoom(params, trtc::TRTCAppSceneVideoCall);
    writeLblLog("end OnEnterRoom_Click roomid: 110");
}

void UBtnTRTCUserWidget::onRenderVideoFrame(const char *userId, trtc::TRTCVideoStreamType streamType, trtc::TRTCVideoFrame *videoFrame) {
    int frameLength = videoFrame->length;
    if (localPreviewImage != nullptr && frameLength > 1) {
        // 获取到BGRA32 帧数据
        UpdateBuffer(videoFrame->data,videoFrame->width,videoFrame->height,frameLength,true);
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
        if (localBufferSize== NewSize)
        {
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
    }
	else
    {

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
    }
    else
    {

    }
}

void UBtnTRTCUserWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime) {
    Super::NativeTick(MyGeometry, DeltaTime);
	FScopeLock lock(&localMutex);
    if(localBuffer == nullptr)
        return;
    if (localUpdateTextureRegion->Width != localWidth ||
        localUpdateTextureRegion->Height != localHeight)
    {
        auto NewUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, localWidth, localHeight);
        // PF_R8G8B8A8
        // macos PF_B8G8R8A8 --> TRTCVideoPixelFormat_BGRA32 验证通过
        auto NewRenderTargetTexture = UTexture2D::CreateTransient(localWidth, localHeight);
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
void UBtnTRTCUserWidget::NativeConstruct() {
    Super::NativeConstruct();
    pTRTCCloud = getTRTCShareInstance();
    pTRTCCloud->addCallback(this);
    std::string version = pTRTCCloud->getSDKVersion();
    btnEnterroom->OnClicked.AddDynamic(this, &UBtnTRTCUserWidget::OnEnterRoom_Click);
    btnTrtcTest->OnClicked.AddDynamic(this, &UBtnTRTCUserWidget::handleTestButtonClick);
    btnLocalPreview->OnClicked.AddDynamic(this, &UBtnTRTCUserWidget::OnStartLocalPreview_Click);
    btnStopPreview->OnClicked.AddDynamic(this, &UBtnTRTCUserWidget::OnStopLocalPreview_Click);
    writeLblLog(version.c_str());
    //TODO:
	localWidth = 640;
	localHeight = 368;

	localRenderTargetTexture = UTexture2D::CreateTransient(localWidth, localHeight );
	localRenderTargetTexture->UpdateResource();

	localBufferSize= localWidth * localHeight * 4;
	localBuffer = new uint8[localBufferSize];
	for (uint32 i = 0; i < localWidth * localWidth; ++i)
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
}

void UBtnTRTCUserWidget::NativeDestruct() {
    Super::NativeDestruct();
    if (pTRTCCloud != nullptr) {
        pTRTCCloud->removeCallback(this);
        pTRTCCloud->destroyTRTCShareInstance();
        pTRTCCloud = nullptr;
    }
    delete[] localBuffer;
	delete localUpdateTextureRegion;
}
void UBtnTRTCUserWidget::writeLblLog(const char * logStr) {
    std::string stdStrLog(logStr);
    FString log = stdStrLog.c_str();
    UE_LOG(LogTemp,Log,TEXT("==> %s"), *log);
#if PLATFORM_IOS
    //NSLog(@"--------");
#endif
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
        pTRTCCloud->setRemoteVideoRenderCallback(userId,trtc::TRTCVideoPixelFormat_BGRA32,trtc::TRTCVideoBufferType_Buffer, this);
    }else{
        pTRTCCloud->muteRemoteVideoStream(userId, true);
        
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
