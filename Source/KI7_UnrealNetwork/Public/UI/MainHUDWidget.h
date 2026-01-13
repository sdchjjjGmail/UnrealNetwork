// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDWidget.generated.h"

class UDataLineWidget;
/**
 * 
 */
UCLASS()
class KI7_UNREALNETWORK_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdateLevel(int32 NewLevel);
	
	UFUNCTION(BlueprintCallable)
	void UpdateExp(float NewExp);
	
protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UDataLineWidget> Level = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UDataLineWidget> Exp = nullptr;
};
