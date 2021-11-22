// Fill out your copyright notice in the Description page of Project Settings.


#include "BtnTRTCUserWidget.h"
#include "Engine/Engine.h"
#include <string>
#include "../TRTCSDK/include/ITRTCCloud.h"

void UBtnTRTCUserWidget::handleInitButtonClick()
{
    //std::string perfect = std::to_string(trtc::TXLiteAVError.ERR_NULL);
    UE_LOG(LogTemp,Warning, TEXT("++++++++++handleInitButtonClick+++++"));
    if (TextBlock_1 != nullptr) {
        TextBlock_1->SetText(FText::FromString("====handleInitButtonClick======"));
    }else{
        UE_LOG(LogTemp,Warning, TEXT("TextBlock_1 not find"));
    }
}

void UBtnTRTCUserWidget::handleInitButtonPress()
{
    UE_LOG(LogTemp,Warning, TEXT("+handleInitButtonPress+++++++++-------++++"));
    if (TextBlock_1 != nullptr) {
        TextBlock_1->SetText(FText::FromString("handleInitButtonPress"));
    }else{
        UE_LOG(LogTemp,Warning, TEXT("TextBlock_1 not find"));
    }
}

void UBtnTRTCUserWidget::OnJButton_1Click()
{
    UE_LOG(LogTemp, Warning, TEXT("UBtnTRTCUserWidget ====== OnJButton_1Click ======"));
    if (TextBlock_1 != nullptr) {
        TextBlock_1->SetText(FText::FromString("====OnJButton_1Click======"));
    }else{
        UE_LOG(LogTemp,Warning, TEXT("TextBlock_1 not find"));
    }
}

void UBtnTRTCUserWidget::NativeConstruct() {
    Super::NativeConstruct();
    UE_LOG(LogTemp, Warning, TEXT("UBtnTRTCUserWidget ====== NativeConstruct ======"));
    liteav::ITRTCCloud * pTRTCCloud = getTRTCShareInstance();
    std::string version = pTRTCCloud->getSDKVersion();
    Button_1->OnClicked.AddDynamic(this, &UBtnTRTCUserWidget::OnJButton_1Click);
    if (TextBlock_1 != nullptr) {
        TextBlock_1->SetText(FText::FromString(version.c_str()));
    }else{
        UE_LOG(LogTemp,Warning, TEXT("TextBlock_1 not find"));
    }
}

void UBtnTRTCUserWidget::NativeDestruct() {
    Super::NativeDestruct();
}
