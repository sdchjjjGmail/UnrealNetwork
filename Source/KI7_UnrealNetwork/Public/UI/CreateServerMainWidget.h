// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateServerMainWidget.generated.h"

class UButton;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class KI7_UNREALNETWORK_API UCreateServerMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCreateButtonClicked();

	UFUNCTION()
	void OnJoinButtonClicked();

	UFUNCTION()
	void OnDisconnectButtonClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CreateButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> JoinButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> DisconnectButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> InputIPAddress = nullptr;
};
