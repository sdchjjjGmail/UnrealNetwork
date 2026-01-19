// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Practice/NameDisplayWidget.h"
#include "Components/TextBlock.h"
#include "Framework/TestPlayerState.h"

void UNameDisplayWidget::SetMyDisplayName(FString NewName)
{
	NameDisplayText->SetText(FText::FromString(NewName));
}

void UNameDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
