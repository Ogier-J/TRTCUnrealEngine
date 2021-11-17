// Fill out your copyright notice in the Description page of Project Settings.


#include "TrtcTestUserWidget.h"

void TrtcTestUserWidget::handleInitButtonClick()
{
    UE_LOG(LogTemp,Warning, TEXT("hello word+++++"));
    FPlatformMisc::MessageBoxExt(EAppMsgType::Ok, TEXT("start"), TEXT("test"));
    UE_LOG(LogTemp,Warning, TEXT("++++++++++hello word+++++"));
}
