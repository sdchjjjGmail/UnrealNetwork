// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TestCharacter.h"
#include "NetworkRoll.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_NETWORK_API ANetworkRoll : public ATestCharacter
{
	GENERATED_BODY()
	
protected:
	virtual void PrintNetworkInfo() override;

	virtual void Tick(float DeltaTime) override;
};
