// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStateMainHudWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class KI7_UNREALNETWORK_API UGameStateMainHudWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Game Data")
	void UpdateTimeDisplay();

	UFUNCTION(BlueprintCallable, Category = "Game Data")
	void UpdateGameOverDisplay();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ElapsedTimeText = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> RemainingTimeText = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> GameOverText = nullptr;

private:
	TWeakObjectPtr<class ATestGameState> CachedGameState = nullptr;

};
