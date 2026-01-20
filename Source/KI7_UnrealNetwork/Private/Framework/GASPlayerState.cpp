// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GASPlayerState.h"
#include "GAS/TestAttributeSet.h"
#include "AbilitySystemComponent.h"

AGASPlayerState::AGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	ResourceAS = CreateDefaultSubobject<UTestAttributeSet>(TEXT("Resource"));

	//SetNetUpdateFrequency(100.0f);
}

void AGASPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetDefaultAbilitySystemData();
	}
}

void AGASPlayerState::SetDefaultAbilitySystemData()
{
	if (HasAuthority() && ASC)
	{
		// DefaultAbilities 순회 : 키는 EAbilityInputID, 값은 어빌리티 클래스
		for (const TPair<EAbilityInputID, TSubclassOf<UGameplayAbility>>& Pair : DefaultAbilities)
		{
			if (Pair.Value)
			{
				ASC->GiveAbility(
					FGameplayAbilitySpec(Pair.Value, 1, static_cast<int32>(Pair.Key), this)
				);
			}
		}

		for (TSubclassOf<UGameplayEffect>& EffectClass : DefaultEffects)
		{
			if (EffectClass)
			{
				FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
				EffectContext.AddSourceObject(this);
				FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, 1, EffectContext);

				if (SpecHandle.IsValid())
				{
					ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
		}
	}
}
