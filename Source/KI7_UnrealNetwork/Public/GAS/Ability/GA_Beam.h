// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Beam.generated.h"

/**
 * 
 */
UCLASS()
class KI7_UNREALNETWORK_API UGA_Beam : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Beam();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateCancelAbility) override;

	virtual void InputReleased(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility, bool bWasCancelled) override;

protected:


private:
	void BeamTick();
	void PerformBeamTrace(FHitResult& OutHit, FVector& OutEndPoint);
	void ApplyDamageToTarget(UAbilitySystemComponent* TargetASC);
	//void RemoveDamageFromCurrentTarget();
	void CleanupBeam();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ablility|Beam")
	TSubclassOf<UGameplayEffect> DamageEffectClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Ablility|Beam")
	TWeakObjectPtr<class UNiagaraSystem> BeamNiagaraSystem = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Ablility|Beam")
	FName BeamEndParamName = FName("BeamEnd");

	UPROPERTY(EditDefaultsOnly, Category = "Ablility|Beam")
	float BeamRange = 300.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Ablility|Beam")
	float DamagePerSecond = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Ablility|Beam")
	float TickRate = 0.1f;

private:
	UPROPERTY()
	TWeakObjectPtr<class UNiagaraComponent> BeamNiagaraComponent;

	//FActiveGameplayEffectHandle CurrentEffectHandle;
	//AActor* CurrentTarget = nullptr;

	FTimerHandle BeamTickTimer;

	class AGASPlayerCharacter* AvatarCharacter = nullptr;
};
