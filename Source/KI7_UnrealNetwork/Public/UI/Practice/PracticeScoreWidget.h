// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PracticeScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class KI7_UNREALNETWORK_API UPracticeScoreWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetMyScoreText(int32 InValue);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnMyNameTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

private:
	void SearchPlayerState();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEditableTextBox> InputNameText = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextScore = nullptr;

private:
	FTimerHandle PlayerStateSearchHandle;

	int32 RetryCount = 0;
	const int32 MaxRetry = 100;
};
