// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUDWidget.h"
#include "UI/DataLineWidget.h"

void UMainHUDWidget::UpdateLevel(int32 NewLevel)
{
	Level->UpdateIntValue(NewLevel);
}

void UMainHUDWidget::UpdateExp(float NewExp)
{
	Exp->UpdateFloatValue(NewExp);
}
