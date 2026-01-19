// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NameDisplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class KI7_UNREALNETWORK_API UNameDisplayWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetMyDisplayName(FString NewName);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> NameDisplayText = nullptr;
};
