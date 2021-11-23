// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Modules/ModuleManager.h"
#include "ITRTCCloud.h"
#include "BtnTRTCUserWidget.generated.h"
/**
 * 
 */
UCLASS()
class UBtnTRTCUserWidget : public UUserWidget,public trtc::ITRTCVideoRenderCallback
{
	GENERATED_BODY()
public:
    trtc::ITRTCCloud * pTRTCCloud;
    
    UFUNCTION(BlueprintCallable, Category ="initFunction")
        void handleInitButtonClick();
    
    UFUNCTION(BlueprintCallable, Category ="initFunction")
        void handleInitButtonPress();
    
    UPROPERTY(VisibleAnywhere, Meta = (BindWidget))
        UTextBlock* TextBlock_1;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
        UButton* Button_1 = nullptr;
    
    UFUNCTION(BlueprintCallable)
        void OnJButton_1Click();
    
    void NativeConstruct() override;

    void NativeDestruct() override;
    // 自定义视频渲染回调
    void onRenderVideoFrame(const char *userId, trtc::TRTCVideoStreamType streamType, trtc::TRTCVideoFrame *frame) override;
        
};
