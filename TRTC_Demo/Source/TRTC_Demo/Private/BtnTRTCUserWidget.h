// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "SlateOptMacros.h"
#include "Engine/Texture2D.h"
#include "Components/TextBlock.h"
#include "Modules/ModuleManager.h"
#if PLATFORM_MAC
#include "GenerateTestUserSig.h"
#endif
#include "ITRTCCloud.h"
#include <map>
#include <mutex>
#include "BtnTRTCUserWidget.generated.h"
struct VideoTextureFrame {
    VideoTextureFrame():_texture(nullptr),_region(nullptr),_fresh(false),_width(0),_height(0),_uid(0) {}
    UTexture2D* _texture;
    FSlateBrush Brush;
    std::unique_ptr<FUpdateTextureRegion2D> _region;
    bool _fresh;
    int _width;
    int _height;
    const char * _uid;
};
/**
 * 
 */
UCLASS()
class UBtnTRTCUserWidget : public UUserWidget,public trtc::ITRTCVideoRenderCallback, public trtc::ITRTCCloudCallback
{
	GENERATED_BODY()
private:
    void onExitRoom(int reason) override;
    void onEnterRoom(int result) override;
    void onUserVideoAvailable(const char *userId, bool available) override;
    void onError(TXLiteAVError errCode, const char *errMsg, void *extraInfo) override;
    void onWarning(TXLiteAVWarning warningCode, const char *warningMsg, void *extraInfo) override;
    void writeLblLog(const char *log);
    void writeCallbackLog(const char *log);
    trtc::ITRTCCloud * pTRTCCloud;
    
public:
    uint8* Buffer = nullptr;
	uint32_t Width = 0;
	uint32_t Height = 0;
	uint32 BufferSize = 0;
	
    void UpdateBuffer(
		char* RGBBuffer,
		uint32_t Width,
		uint32_t Height,
		uint32_t Size);

	void ResetBuffer();

public:
    UFUNCTION(BlueprintCallable, Category ="trtcDemoFunction")
        void handleInitButtonClick();
    UFUNCTION(BlueprintCallable, Category ="trtcDemoFunction")
        void OnEnterRoom_Click();
    UFUNCTION(BlueprintCallable, Category ="trtcDemoFunction")
        void OnStartLocalPreview_Click();
    UFUNCTION(BlueprintCallable, Category ="trtcDemoFunction")
        void OnStopLocalPreview_Click();

    UPROPERTY(VisibleAnywhere, Meta = (BindWidget))
        UTextBlock* txtLog;

    UPROPERTY(VisibleAnywhere, Meta = (BindWidget))
        UTextBlock* txtcallBack;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
        UButton* btnEnterroom = nullptr;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
        UButton* btnInitTrtc = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
        UButton* btnLocalPreview = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
        UButton* btnStopPreview = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
        UImage* remoteImage= nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
        UImage* localPreviewImage = nullptr;

    UPROPERTY(EditDefaultsOnly)
		UTexture2D* RenderTargetTexture = nullptr;
    
    FUpdateTextureRegion2D* UpdateTextureRegion = nullptr;

	FSlateBrush Brush;

	FCriticalSection Mutex;

    void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
    
    void NativeConstruct() override;

    void NativeDestruct() override;
    // 自定义视频渲染回调
    void onRenderVideoFrame(const char *userId, trtc::TRTCVideoStreamType streamType, trtc::TRTCVideoFrame *frame) override;
        
};
