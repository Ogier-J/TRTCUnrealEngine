// Fill out your copyright notice in the Description page of Project Settings.


#include "BtnTRTCUserWidget.h"
#include "Engine/Engine.h"
#include <string>

void UBtnTRTCUserWidget::handleInitButtonClick()
{
    
}

void UBtnTRTCUserWidget::OnEnterRoom_Click()
{
    pTRTCCloud->startLocalPreview(nullptr);
    pTRTCCloud->setLocalVideoRenderCallback(trtc::TRTCVideoPixelFormat_I420, trtc::TRTCVideoBufferType_Buffer, this);
    
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
    if (txtLog != nullptr) {
        txtLog->SetText(FText::FromString(version.c_str()));
    }else{
        UE_LOG(LogTemp,Warning, TEXT("txtLog not find"));
    }
}

void UBtnTRTCUserWidget::NativeDestruct() {
    Super::NativeDestruct();
    if (pTRTCCloud != nullptr) {
        pTRTCCloud->removeCallback(this);
        pTRTCCloud = nullptr;
   }
}

void  UBtnTRTCUserWidget::onUserVideoAvailable(const char *userId, bool available) {
}
void UBtnTRTCUserWidget::onWarning(TXLiteAVWarning warningCode, const char *warningMsg, void *extraInfo) { }
void UBtnTRTCUserWidget::onError(TXLiteAVError errCode, const char *errMsg, void *extraInfo) { }
void UBtnTRTCUserWidget::onEnterRoom(int result) { }
void UBtnTRTCUserWidget::onExitRoom(int reason) { }
