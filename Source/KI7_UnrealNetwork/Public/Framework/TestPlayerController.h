// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

DECLARE_DELEGATE(FOnAbilityPress)
/**
 * 
 */
UCLASS()
class KI7_UNREALNETWORK_API ATestPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;	
	virtual void SetupInputComponent() override;

	void OnInputAbility1();

public:
	FOnAbilityPress OnAbility1Press;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> AbilityMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Ability1;

};
