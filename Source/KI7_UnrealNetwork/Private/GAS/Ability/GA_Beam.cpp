// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/GA_Beam.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/Character.h"
#include "Characters/GASPlayerCharacter.h"

UGA_Beam::UGA_Beam()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_Beam::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (ActorInfo->AvatarActor.IsValid())
	{
		AvatarCharacter = Cast<AGASPlayerCharacter>(ActorInfo->AvatarActor.Get());
	}

	if (AvatarCharacter)
	{
		AvatarCharacter->Server_RequestIgnoreMoveInput(true);
		if (BeamNiagaraSystem.IsValid())
		{
			AvatarCharacter->Multicast_StartBeam(BeamNiagaraSystem.Get(), BeamEndParamName);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(
		BeamTickTimer,
		this,
		&UGA_Beam::BeamTick,
		TickRate, true, 0
	);
}

void UGA_Beam::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	CleanupBeam();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Beam::InputReleased(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_Beam::BeamTick()
{
	if (AvatarCharacter)
	{
		FHitResult HitResult;
		FVector EndPoint;
		PerformBeamTrace(HitResult, EndPoint);

		AvatarCharacter->Multicast_UpdateBeamEndPoint(BeamEndParamName, EndPoint);

		if (AvatarCharacter->HasAuthority())
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor && HitActor != AvatarCharacter)
			{
				UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
				ApplyDamageToTarget(TargetASC);
			}
		}
	}
}

void UGA_Beam::PerformBeamTrace(FHitResult& OutHit, FVector& OutEndPoint)
{
	if (AvatarCharacter)
	{
		FVector Start = AvatarCharacter->GetActorLocation();
		OutEndPoint = Start + AvatarCharacter->GetActorForwardVector() * BeamRange;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(AvatarCharacter);
		GetWorld()->LineTraceSingleByChannel(
			OutHit,
			Start, OutEndPoint,
			ECC_Camera, QueryParams);

		if (OutHit.bBlockingHit)
		{
			OutEndPoint = OutHit.ImpactPoint;
		}
	}
}

void UGA_Beam::ApplyDamageToTarget(UAbilitySystemComponent* TargetASC)
{
	if (TargetASC && DamageEffectClass)
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
		if (SourceASC)
		{
			FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);
			FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
				DamageEffectClass, GetAbilityLevel(), EffectContext
			);
			if (SpecHandle.IsValid())
			{
				SpecHandle.Data->SetSetByCallerMagnitude(
					FGameplayTag::RequestGameplayTag(FName("Data.Damage")),
					-DamagePerSecond * TickRate);
				SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);

				// DamageEffect
				// - 스택이 1개까지(Duration으로 할 경우 필요)
				// - SetByCaller로 Health 감소량 설정 가능해야함
			}
		}
	}
}

//void UGA_Beam::RemoveDamageFormCurrentTarget()
//{
//}

void UGA_Beam::CleanupBeam()
{
	if (BeamTickTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(BeamTickTimer);
	}

	if (AvatarCharacter)
	{
		AvatarCharacter->Multicast_StopBeam();
		AvatarCharacter->Server_RequestIgnoreMoveInput(false);
		//if (APlayerController* PC = Cast<APlayerController>(AvatarCharacter->GetController()))
		//{
		//	UE_LOG(LogTemp, Log, TEXT("[%d] CleanupBeam"), PC->GetPlayerState<APlayerState>()->GetPlayerId());
		//	PC->ClientIgnoreMoveInput(false);
		//}
	}

	AvatarCharacter = nullptr;
}