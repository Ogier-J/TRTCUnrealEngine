// Fill out your copyright notice in the Description page of Project Settings.


#include "BtnTRTCUserWidget.h"
#include "Engine/Engine.h"
#include <string>
#ifdef __APPLE__

#endif

void UBtnTRTCUserWidget::handleInitButtonClick()
{
    writeLblLog("start handleInitButtonClick");
}

void UBtnTRTCUserWidget::OnEnterRoom_Click()
{
    writeLblLog("start OnEnterRoom_Click");
    // pTRTCCloud->startLocalPreview(nullptr);
    pTRTCCloud->setLocalVideoRenderCallback(trtc::TRTCVideoPixelFormat_I420, trtc::TRTCVideoBufferType_Buffer, this);
    
    // 构造进房参数
    trtc::TRTCParams params;
    trtc::TRTCRoleType type = static_cast<trtc::TRTCRoleType>(20);
    params.role = type;
    int SDKAppID = 0;
    
    params.sdkAppId = SDKAppID;
    std::string uid = "";
    params.userId = uid.c_str();
    std::string strRoomId = "10";
    params.strRoomId = strRoomId.c_str();
    std::string userSig = "";
#ifdef __APPLE__
    // 暂时注释
//    userSig = GenerateTestUserSig().genTestUserSig(uid.c_str(), SDKAppID, "SECRETKEY");
#endif
    params.userSig = userSig.c_str();

    /* 可以不填写, "321123"只是简单测试使用
     * 推荐方案是：进房时使用 “sdkappid_roomid_userid_main” 作为 streamid，这样比较好辨认且不会在您的多个应用中发生冲突 */
    params.streamId = "321123";
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
    writeLblLog(version.c_str());
}

void UBtnTRTCUserWidget::NativeDestruct() {
    Super::NativeDestruct();
    if (pTRTCCloud != nullptr) {
        pTRTCCloud->removeCallback(this);
        pTRTCCloud = nullptr;
   }
}
void UBtnTRTCUserWidget::writeLblLog(const char *logStr) {
    UE_LOG(LogTemp, Warning, TEXT("writeLblLog log: %s"), logStr);
    if (txtLog != nullptr) {
        std::string stdStrLog(logStr);
        AsyncTask(ENamedThreads::GameThread, [=]()
        {
            FString log = stdStrLog.c_str();
            txtLog->SetText(FText::FromString(log));
        });
        
    }else{
        UE_LOG(LogTemp,Warning, TEXT("txtLog not find"));
    }
}
void  UBtnTRTCUserWidget::onUserVideoAvailable(const char *userId, bool available) {
}
void UBtnTRTCUserWidget::onWarning(TXLiteAVWarning warningCode, const char *warningMsg, void *extraInfo) {
    writeLblLog(warningMsg);
}
void UBtnTRTCUserWidget::onError(TXLiteAVError errCode, const char *errMsg, void *extraInfo) {
    writeLblLog(errMsg);
}
void UBtnTRTCUserWidget::onEnterRoom(int result) {
}
void UBtnTRTCUserWidget::onExitRoom(int reason) { }
