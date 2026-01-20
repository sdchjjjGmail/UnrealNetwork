// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "BilboardWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class KI7_UNREALNETWORK_API UBilboardWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	UBilboardWidgetComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(
		float DeltaTime,
		enum ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;
};
