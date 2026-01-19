// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/GA_Shoot.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

UGA_Shoot::UGA_Shoot()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Shoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!HitEffectClass)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AActor* Avatar = ActorInfo->AvatarActor.Get();
	FHitResult HitResult;
	if (HitScan(HitResult, Avatar))
	{
		// 명중
		DrawDebugLine(GetWorld(), Avatar->GetActorLocation(), HitResult.ImpactPoint, FColor::Yellow, false, 0.1f, 0, 1.0f);
		DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 0.1f);

		AActor* Target = HitResult.GetActor();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
		if (TargetASC && HitEffectClass)
		{
			FGameplayEffectContextHandle Context = TargetASC->MakeEffectContext();
			Context.AddHitResult(HitResult);
			Context.AddInstigator(Avatar->GetInstigator(), Avatar);

			FGameplayEffectSpecHandle Spec = TargetASC->MakeOutgoingSpec(HitEffectClass, 1, Context);
			if (Spec.IsValid())
			{
				TargetASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
			}
		}
	}
	else
	{
		// 빗나감
		DrawDebugLine(GetWorld(), Avatar->GetActorLocation(), HitResult.ImpactPoint, FColor::Green, false, 0.1f, 0, 1.0f);
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

bool UGA_Shoot::HitScan(FHitResult& OutHitResult, AActor* Avatar)
{
	FVector Start = Avatar->GetActorLocation();
	FVector End = Start + Avatar->GetActorForwardVector() * ShootRange;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Avatar);	

	return GetWorld()->LineTraceSingleByObjectType(OutHitResult, Start, End, ECC_Pawn, QueryParams);
}
