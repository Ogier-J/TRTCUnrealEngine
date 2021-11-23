// Fill out your copyright notice in the Description page of Project Settings.


#include "BtnTRTCUserWidget.h"
#include "Engine/Engine.h"
#include <string>

void UBtnTRTCUserWidget::handleInitButtonClick()
{
    writeLblLog("start handleInitButtonClick");
}

void UBtnTRTCUserWidget::OnEnterRoom_Click()
{
    writeLblLog("start OnEnterRoom_Click");
    pTRTCCloud->startLocalPreview(nullptr);
    pTRTCCloud->setLocalVideoRenderCallback(trtc::TRTCVideoPixelFormat_I420, trtc::TRTCVideoBufferType_Buffer, this);
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
void UBtnTRTCUserWidget::writeLblLog(const char *log) {
    UE_LOG(LogTemp, Warning, TEXT("writeLblLog log: %d"), log);
    if (txtLog != nullptr) {
        txtLog->SetText(FText::FromString(log));
    }else{
        UE_LOG(LogTemp,Warning, TEXT("txtLog not find"));
    }
}
void  UBtnTRTCUserWidget::onUserVideoAvailable(const char *userId, bool available) {
}
void UBtnTRTCUserWidget::onWarning(TXLiteAVWarning warningCode, const char *warningMsg, void *extraInfo) { }
void UBtnTRTCUserWidget::onError(TXLiteAVError errCode, const char *errMsg, void *extraInfo) { }
void UBtnTRTCUserWidget::onEnterRoom(int result) { }
void UBtnTRTCUserWidget::onExitRoom(int reason) { }
