// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Shoot.generated.h"

/**
 * 
 */
UCLASS()
class KI7_UNREALNETWORK_API UGA_Shoot : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Shoot();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	bool HitScan(FHitResult& OutHitResult, AActor* Avatar);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ablility|Shoot")
	TSubclassOf<UGameplayEffect> HitEffectClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Ablility|Shoot")
	float ShootRange = 1000.0f;
};
