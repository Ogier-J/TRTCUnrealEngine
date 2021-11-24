// Fill out your copyright notice in the Description page of Project Settings.


#include "BtnTRTCUserWidget.h"
#include "Engine/Engine.h"
#include <string>
#include "Defs.h"
#ifdef __APPLE__

#endif

void UBtnTRTCUserWidget::handleInitButtonClick() {
    writeLblLog("start handleInitButtonClick");
}
void UBtnTRTCUserWidget::OnStartLocalPreview_Click() {
    writeLblLog("start OnStartLocalPreview_Click");
    pTRTCCloud->startLocalPreview(nullptr);
    pTRTCCloud->setLocalVideoRenderCallback(trtc::TRTCVideoPixelFormat_I420, trtc::TRTCVideoBufferType_Buffer, this);
    writeLblLog("end OnStartLocalPreview_Click");
}
void UBtnTRTCUserWidget::OnEnterRoom_Click() {
    writeLblLog("start OnEnterRoom_Click");
    // 构造进房参数
    trtc::TRTCParams params;
    trtc::TRTCRoleType type = static_cast<trtc::TRTCRoleType>(20);
    params.role = type;
    
    params.sdkAppId = SDKAppID;
    params.userId = testUserId;
    std::string strRoomId = "10";
    params.strRoomId = strRoomId.c_str();
#ifdef __APPLE__
    // 暂时注释
//    userSig = GenerateTestUserSig().genTestUserSig(uid.c_str(), SDKAppID, SECRETKEY);
#endif
    params.userSig = testUserSig;
    trtc::TRTCAppScene style = static_cast<trtc::TRTCAppScene>(0);
    // 进房
    pTRTCCloud->enterRoom(params, style);
    writeLblLog("end OnEnterRoom_Click");
}
void UBtnTRTCUserWidget::onRenderVideoFrame(const char *userId, trtc::TRTCVideoStreamType streamType, trtc::TRTCVideoFrame *frame) {
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
    if (txtLog != nullptr) {
        std::string stdStrLog(logStr);
        AsyncTask(ENamedThreads::GameThread, [=]() {
            FString log = stdStrLog.c_str();
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
    if (txtcallBack != nullptr) {
        std::string stdStrLog(logStr);
        AsyncTask(ENamedThreads::GameThread, [=]() {
            FString log = stdStrLog.c_str();
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
}
void UBtnTRTCUserWidget::onExitRoom(int reason) {
    writeCallbackLog("onExitRoom");
}
