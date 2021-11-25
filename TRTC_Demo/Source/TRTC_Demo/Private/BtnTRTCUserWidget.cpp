// Fill out your copyright notice in the Description page of Project Settings.


#include "BtnTRTCUserWidget.h"
#include "Engine/Engine.h"
#include <string>
#include "DebugDefs.h"
#ifdef __APPLE__

#endif

void UBtnTRTCUserWidget::handleInitButtonClick() {
    writeLblLog("start handleInitButtonClick");
}

void UBtnTRTCUserWidget::OnStartLocalPreview_Click() {
    writeLblLog("start OnStartLocalPreview_Click");
    
#if PLATFORM_IOS || PLATFORM_ANDROID
    pTRTCCloud->startLocalPreview(true, nullptr);
#else
    pTRTCCloud->startLocalPreview(nullptr);
#endif
    pTRTCCloud->setLocalVideoRenderCallback(trtc::TRTCVideoPixelFormat_RGBA32
, trtc::TRTCVideoBufferType_Buffer, this);
    writeLblLog("end OnStartLocalPreview_Click");
    localPreviewImage->SetBrush(_users_texture_frame_map["0"].Brush);
}
void UBtnTRTCUserWidget::OnEnterRoom_Click() {
    writeLblLog("start OnEnterRoom_Click roomid: 110");
    // 构造进房参数
    trtc::TRTCParams params;
    trtc::TRTCRoleType type = static_cast<trtc::TRTCRoleType>(20);
    params.role = type;
    
    params.sdkAppId = SDKAppID;
    params.userId = testUserId;
    std::string strRoomId = "110";
    // params.strRoomId = strRoomId.c_str();
    params.roomId = 110;
    // 暂时指支持macos。
    const char * userSig = GenerateTestUserSig().genTestUserSig(testUserId, SDKAppID, SECRETKEY);
    params.userSig = userSig;
    params.userDefineRecordId = nullptr;
    trtc::TRTCAppScene style = static_cast<trtc::TRTCAppScene>(0);
    // 进房
    pTRTCCloud->enterRoom(params, style);
    writeLblLog("end OnEnterRoom_Click roomid: 110");
}
void UBtnTRTCUserWidget::onRenderVideoFrame(const char *userId, trtc::TRTCVideoStreamType streamType, trtc::TRTCVideoFrame *videoFrame) {
    std::lock_guard<std::mutex> lock(_mutex);
//    UE_LOG(LogTemp,Log,TEXT("onRenderVideoFrame width: %d , length : %d , data : %s"), videoFrame->width, videoFrame->length,videoFrame->data);
    if (localPreviewImage != nullptr) {
        if( !_users_texture_frame_map["0"]._texture ||
           _users_texture_frame_map["0"]._texture->GetSizeX() != videoFrame->width ||
           _users_texture_frame_map["0"]._texture->GetSizeY() != videoFrame->height)
        {
            _users_texture_frame_map["0"]._region.reset( new FUpdateTextureRegion2D(0, 0, 0, 0, (uint32)videoFrame->width, (uint32)videoFrame->height));
            _users_texture_frame_map["0"]._texture = UTexture2D::CreateTransient( videoFrame->width, videoFrame->height, PF_R8G8B8A8 );
            _users_texture_frame_map["0"]._texture->UpdateResource();
            _users_texture_frame_map["0"].Brush.SetResourceObject(_users_texture_frame_map["0"]._texture);
        }
        int frameLength = videoFrame->length;
        uint8_t *slidePressure = new uint8_t[frameLength];
        memcpy(slidePressure, videoFrame->data, frameLength);
        _users_texture_frame_map["0"]._texture->UpdateTextureRegions( 0, 1, _users_texture_frame_map["0"]._region.get(), frameLength, (uint32)argbPixSize, slidePressure);
        
        _users_texture_frame_map["0"]._width = videoFrame->width;
        _users_texture_frame_map["0"]._height = videoFrame->height;
        _users_texture_frame_map["0"]._uid = "0";
        _users_texture_frame_map["0"]._fresh = true;
    }
}
void UBtnTRTCUserWidget::onTick() {
    std::lock_guard<std::mutex> lock(_mutex);
    if(_users_texture_frame_map["0"]._texture && _users_texture_frame_map["0"]._fresh) {
        localPreviewImage->SetBrush(_users_texture_frame_map["0"].Brush);
    }
}
void UBtnTRTCUserWidget::NativeConstruct() {
    Super::NativeConstruct();
    pTRTCCloud = getTRTCShareInstance();
    pTRTCCloud->addCallback(this);
    std::string version = pTRTCCloud->getSDKVersion();
    btnEnterroom->OnClicked.AddDynamic(this, &UBtnTRTCUserWidget::OnEnterRoom_Click);
    btnInitTrtc->OnClicked.AddDynamic(this, &UBtnTRTCUserWidget::handleInitButtonClick);
    btnLocalPreview->OnClicked.AddDynamic(this, &UBtnTRTCUserWidget::OnStartLocalPreview_Click);
    writeLblLog(version.c_str());
}

void UBtnTRTCUserWidget::NativeDestruct() {
    if (pTRTCCloud != nullptr) {
        pTRTCCloud->removeCallback(this);
        pTRTCCloud->destroyTRTCShareInstance();
        pTRTCCloud = nullptr;
    }
    Super::NativeDestruct();
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
    writeCallbackLog(userId);
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
