// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "SlateOptMacros.h"
#include "Engine/Texture2D.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Modules/ModuleManager.h"
// 这段是为ue4 打包的时候不报错。
#if PLATFORM_MAC || PLATFORM_IOS || PLATFORM_WINDOWS
#include "GenerateTestUserSig.h"
#endif
#include "ITRTCCloud.h"
#include "TRTCTypeDef.h"
#include <map>
#include <mutex>
#include "BtnTRTCUserWidget.generated.h"
struct VideoTextureFrame {
    UTexture2D* Texture;
    FSlateBrush Brush;
    std::unique_ptr<FUpdateTextureRegion2D> UpdateTextureRegion;
    bool Fresh;
    int Width;
    int Height;
    const char * userId;
    VideoTextureFrame():Texture(nullptr),UpdateTextureRegion(nullptr),Fresh(false),Width(0),Height(0),userId(0) {}
};
#if PLATFORM_WINDOWS
#define UpdateResource UpdateResource
#endif
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
    UFUNCTION(BlueprintCallable, Category ="trtcDemoFunction")
        void handleTestButtonClick();
    UFUNCTION(BlueprintCallable, Category ="trtcDemoFunction")
        void OnEnterRoom_Click();
    UFUNCTION(BlueprintCallable, Category ="trtcDemoFunction")
        void OnStartLocalPreview_Click();
    UFUNCTION(BlueprintCallable, Category ="trtcDemoFunction")
        void OnStopLocalPreview_Click();
    UFUNCTION(BlueprintCallable, Category ="trtcDemoFunction")
        void OnStartScreen_Click();

    UPROPERTY(VisibleAnywhere, Meta = (BindWidget))
        UTextBlock* txtLog;

    UPROPERTY(VisibleAnywhere, Meta = (BindWidget))
        UTextBlock* txtcallBack;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
        UButton* btnEnterroom = nullptr;

    UPROPERTY(VisibleAnywhere, Meta = (BindWidget))
        UEditableTextBox * txtRoomID = nullptr;

    UPROPERTY(VisibleAnywhere, Meta = (BindWidget))
        UEditableTextBox * txtUserId = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
        UButton* btnTrtcTest = nullptr;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
        UButton* BtnScreenCapture = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
        UImage* ImageScreen = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
        UButton* btnLocalPreview = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
        UButton* btnStopPreview = nullptr;

    // 本地视频
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
        UImage* localPreviewImage = nullptr;

    UPROPERTY(EditDefaultsOnly)
		UTexture2D* localRenderTargetTexture = nullptr;
    FUpdateTextureRegion2D* localUpdateTextureRegion = nullptr;
	FSlateBrush localBrush;
	FCriticalSection localMutex;
    uint8* localBuffer = nullptr;
	uint32_t localWidth = 0;
	uint32_t localHeight = 0;
    bool localRefresh = false;
	uint32 localBufferSize = 0;

    // 远端用户345 视频
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
        UImage* remoteImage = nullptr;

    UPROPERTY(EditDefaultsOnly)
		UTexture2D* remoteRenderTargetTexture = nullptr;
    FUpdateTextureRegion2D* remoteUpdateTextureRegion = nullptr;
	FSlateBrush remoteBrush;
	FCriticalSection remoteMutex;
    uint8* remoteBuffer = nullptr;
	uint32_t remoteWidth = 0;
	uint32_t remoteHeight = 0;
    bool remoteRefresh = false;
	uint32 remoteBufferSize = 0;

    // 更新帧数据
    void UpdateBuffer(
		char* RGBBuffer,
		uint32_t Width,
		uint32_t Height,
		uint32_t Size,
        bool isLocal);

	void ResetBuffer(bool isLocal);

    void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
    
    void NativeConstruct() override;

    void NativeDestruct() override;
    // 自定义视频渲染回调
    void onRenderVideoFrame(const char *userId, trtc::TRTCVideoStreamType streamType, trtc::TRTCVideoFrame *frame) override;
        
};
