// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Modules/ModuleManager.h"
#include "BtnTRTCUserWidget.generated.h"
//#include "../../../TRTCSDK/include/ITRTCCloud.h"


/**
 * 
 */
UCLASS()
class UBtnTRTCUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
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
    
};
