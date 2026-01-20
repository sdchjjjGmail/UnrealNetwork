// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GASPlayerCharacter.generated.h"

class UAbilitySystemComponent;
class UTestAttributeSet;
struct FOnAttributeChangeData;

UCLASS()
class KI7_UNREALNETWORK_API AGASPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGASPlayerCharacter();

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_PlayerState() override;

	void OnAbility1Press();

	UFUNCTION(Server, Reliable)
	void Server_ExecuteAbility1();

private:
	void InitializeAbilitySystem();
	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }
	UTestAttributeSet* GetResourceAttributeSet() const { return ResourceAS; }

	UFUNCTION(BlueprintCallable, Category = "GAS|Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|Health")
	float GetMaxHealth() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UBilboardWidgetComponent> Widget = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC = nullptr;

	UPROPERTY()
	TObjectPtr<UTestAttributeSet> ResourceAS = nullptr;

	// 어빌리티 시스템 초기화 했는지 확인용
	bool bAbilitySystemInitialized = false;
};
