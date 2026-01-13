// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DataLineWidget.h"
#include "Components/TextBlock.h"

void UDataLineWidget::UpdateName(FText InName)
{
	Name->SetText(InName);
}

void UDataLineWidget::UpdateIntValue(int32 InValue)
{
	Value->SetText(FText::AsNumber(InValue));
}

void UDataLineWidget::UpdateFloatValue(float InValue)
{
	Value->SetText(FText::AsNumber(static_cast<int32>(InValue)));
}
