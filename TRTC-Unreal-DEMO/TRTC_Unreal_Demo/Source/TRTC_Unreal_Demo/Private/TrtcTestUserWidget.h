// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TrtcTestUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UTrtcTestUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
        UFUNCTION(BlueprintCallable, Category ="initFunction")
    void handleInitButtonClick();
};
