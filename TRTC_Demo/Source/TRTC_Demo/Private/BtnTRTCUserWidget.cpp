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

void UBtnTRTCUserWidget::ResetBuffer()
{
	for (uint32 i = 0; i < Width * Height; ++i)
	{
		Buffer[i * 4 + 0] = 0x32;
		Buffer[i * 4 + 1] = 0x32;
		Buffer[i * 4 + 2] = 0x32;
		Buffer[i * 4 + 3] = 0xFF;
	}
}

void UBtnTRTCUserWidget::onRenderVideoFrame(const char *userId, trtc::TRTCVideoStreamType streamType, trtc::TRTCVideoFrame *videoFrame) {
//    UE_LOG(LogTemp,Log,TEXT("onRenderVideoFrame width: %d , length : %d , data : %s"), videoFrame->width, videoFrame->length,videoFrame->data);
    if (localPreviewImage != nullptr) {
        int frameLength = videoFrame->length;
        // uint8_t *slidePressure = new uint8_t[frameLength];
        // FMemory::Memcpy(slidePressure, videoFrame->data, frameLength);
        UpdateBuffer(videoFrame->data,videoFrame->width,videoFrame->height,frameLength);
    }
}

void UBtnTRTCUserWidget::UpdateBuffer(
	char* RGBBuffer,
	uint32_t NewWidth,
	uint32_t NewHeight,
	uint32_t NewSize)
{
	FScopeLock lock(&Mutex);

	if (!RGBBuffer)
	{
		return;
	}

	if (BufferSize == NewSize)
	{
		std::copy(RGBBuffer, RGBBuffer + NewSize, Buffer);
	}
	else
	{
		delete[] Buffer;
		BufferSize = NewSize;
		Width = NewWidth;
		Height = NewHeight;
		Buffer = new uint8[BufferSize];
		std::copy(RGBBuffer, RGBBuffer + NewSize, Buffer);
	}
}

void UBtnTRTCUserWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime) {
    Super::NativeTick(MyGeometry, DeltaTime);
    
	FScopeLock lock(&Mutex);

	if (UpdateTextureRegion->Width != Width ||
		UpdateTextureRegion->Height != Height)
	{
		auto NewUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, Width, Height);

		auto NewRenderTargetTexture = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8 );
		NewRenderTargetTexture->UpdateResource();
		NewRenderTargetTexture->UpdateTextureRegions(0, 1, NewUpdateTextureRegion, Width * 4, (uint32)4, Buffer);

		Brush.SetResourceObject(NewRenderTargetTexture);
		localPreviewImage->SetBrush(Brush);
		FUpdateTextureRegion2D* TmpUpdateTextureRegion = UpdateTextureRegion;
		RenderTargetTexture = NewRenderTargetTexture;
		UpdateTextureRegion = NewUpdateTextureRegion;
		delete TmpUpdateTextureRegion;
		return;
	}

	RenderTargetTexture->UpdateTextureRegions(0, 1, UpdateTextureRegion, Width * 4, (uint32)4, Buffer);
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
    //TODO:
	Width = 640;
	Height = 360;

	RenderTargetTexture = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8 );
	RenderTargetTexture->UpdateResource();

	BufferSize = Width * Height * 4;
	Buffer = new uint8[BufferSize];
	for (uint32 i = 0; i < Width * Height; ++i)
	{
		Buffer[i * 4 + 0] = 0x32;
		Buffer[i * 4 + 1] = 0x32;
		Buffer[i * 4 + 2] = 0x32;
		Buffer[i * 4 + 3] = 0xFF;
	}
	UpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, Width, Height);
	RenderTargetTexture->UpdateTextureRegions(0, 1, UpdateTextureRegion, Width * 4, (uint32)4, Buffer);

	Brush.SetResourceObject(RenderTargetTexture);
	localPreviewImage->SetBrush(Brush);
}

void UBtnTRTCUserWidget::NativeDestruct() {
    if (pTRTCCloud != nullptr) {
        pTRTCCloud->removeCallback(this);
        pTRTCCloud->destroyTRTCShareInstance();
        pTRTCCloud = nullptr;
    }
    delete[] Buffer;
	delete UpdateTextureRegion;
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
