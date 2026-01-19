// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

class UUserWidget;
/**
 * 
 */
UCLASS()
class KI7_UNREALNETWORK_API AMainHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	inline TWeakObjectPtr<UUserWidget> GetMainHudWidget() const { return MainHudWidget; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainHudWidgetClass = nullptr;

private:
	UPROPERTY()
	TWeakObjectPtr<UUserWidget> MainHudWidget = nullptr;
};
