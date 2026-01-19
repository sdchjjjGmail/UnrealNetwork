// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TestAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

UTestAttributeSet::UTestAttributeSet()
{
	InitHealth(100);
	InitMaxHealth(100);
}

void UTestAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UTestAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTestAttributeSet, MaxHealth, COND_None, REPNOTIFY_OnChanged);

	// REPNOTIFY_Always : 항상 리플리케이션용 함수 실행
	// REPNOTIFY_OnChanged : 값이 변경되었을 때만 리플리케이션용 함수 실행
}

void UTestAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());	// 0 ~ 최대 체력까지
	}

	if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 0);	// 0 이상
	}
}

void UTestAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
		if (FMath::IsNearlyZero(GetHealth()))
		{
			// 사망처리
			UE_LOG(LogTemp, Log, TEXT("사망"));			
		}
	}
}

void UTestAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	// GAS에 어트리뷰트가 리플리케이션 되었다고 알림(필수)
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTestAttributeSet, Health, OldHealth);

	//UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	//ASC->GetAvatarActor();
	//ASC->GetOwnerActor();
}	

void UTestAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTestAttributeSet, MaxHealth, OldHealth);
}
