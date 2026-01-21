// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

DECLARE_DELEGATE(FOnAbilityPress)
DECLARE_DELEGATE(FOnAbility2Input)

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
	void OnInputAbility2Press();
	void OnInputAbility2Release();

public:
	FOnAbilityPress OnAbility1Press;
	FOnAbility2Input OnAbility2Press;
	FOnAbility2Input OnAbility2Release;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> AbilityMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Ability1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Ability2;

};
