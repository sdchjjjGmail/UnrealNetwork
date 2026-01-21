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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_PlayerState() override;

	void OnAbility1Press();
	void OnAbility2Press();
	void OnAbility2Release();

	UFUNCTION(Server, Reliable)
	void Server_ExecuteAbility1();

	UFUNCTION(Server, Reliable)
	void Server_ExecuteAbility2();

	UFUNCTION(Server, Reliable)
	void Server_EndAbility2();

private:
	void InitializeInputBind(AController* ControllerToBind);
	void ClearInputBind();
	void InitializeAbilitySystem();
	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);

public:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartBeam(class UNiagaraSystem* BeamSystem, FName BeamEndParam);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateBeamEndPoint(FName BeamEndParam, const FVector& EndPoint);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StopBeam();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }
	UTestAttributeSet* GetResourceAttributeSet() const { return ResourceAS; }

	UFUNCTION(BlueprintCallable, Category = "GAS|Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|Health")
	float GetMaxHealth() const;

	UFUNCTION(Server, Reliable)
	void Server_RequestIgnoreMoveInput(bool bIgnore);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UBilboardWidgetComponent> Widget = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC = nullptr;

	UPROPERTY()
	TObjectPtr<UTestAttributeSet> ResourceAS = nullptr;

	// 어빌리티 시스템 초기화 했는지 확인용
	bool bAbilitySystemInitialized = false;

	UPROPERTY()
	class UNiagaraComponent* BeamNiagaraComponent = nullptr;
};
