// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GASPlayerCharacter.h"
#include "Framework/GASPlayerState.h"
#include "Framework/TestPlayerController.h"
#include "GAS/TestAttributeSet.h"
#include "GAS/GameAbilitySystemEnums.h"
#include "UI/BilboardWidgetComponent.h"
#include "UI/DataLineWidget.h"
#include "AbilitySystemComponent.h"

// Sets default values
AGASPlayerCharacter::AGASPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Widget = CreateDefaultSubobject<UBilboardWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(GetRootComponent());
}

void AGASPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ATestPlayerController* PC = Cast<ATestPlayerController>(NewController);
	PC->OnAbility1Press.BindUObject(this, &AGASPlayerCharacter::OnAbility1Press);	// 플레이어 컨트롤러의 입력 신호 받기

	InitializeAbilitySystem();	// 서버에서 실행하기 위한 용도
}

void AGASPlayerCharacter::UnPossessed()
{
	ATestPlayerController* PC = Cast<ATestPlayerController>(GetController());
	PC->OnAbility1Press.Unbind();

	Super::UnPossessed();
}

void AGASPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitializeAbilitySystem();	// 클라이언트에서 실행
}

void AGASPlayerCharacter::OnAbility1Press()
{
	if (ASC)
	{
		UE_LOG(LogTemp, Log, TEXT("AGASPlayerCharacter::OnAbility1Press"));
		Server_ExecuteAbility1();
	}
}

void AGASPlayerCharacter::Server_ExecuteAbility1_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("AGASPlayerCharacter::Server_ExecuteAbility1_Implementation"));
	ASC->AbilityLocalInputPressed(static_cast<int32>(EAbilityInputID::Shoot));
}

void AGASPlayerCharacter::InitializeAbilitySystem()
{
	if (bAbilitySystemInitialized)
	{
		return;	// 이전에 초기화한 적이 있으면 종료
	}

	if (AGASPlayerState* PS = GetPlayerState<AGASPlayerState>())
	{
		ASC = PS->GetAbilitySystemComponent();
		ResourceAS = PS->GetResourceAttributeSet();

		if (ASC && ResourceAS)
		{
			ASC->InitAbilityActorInfo(PS, this);
			FOnGameplayAttributeValueChange& onHealthChange =
				ASC->GetGameplayAttributeValueChangeDelegate(UTestAttributeSet::GetHealthAttribute());
			onHealthChange.AddUObject(this, &AGASPlayerCharacter::OnHealthChanged);

			if (Widget && Widget->GetWidget())	// Health UI 첫 초기화
			{
				UDataLineWidget* HealthWidget = Cast<UDataLineWidget>(Widget->GetWidget());
				HealthWidget->UpdateName(FText::AsNumber(ResourceAS->GetHealth()));
				HealthWidget->UpdateIntValue(FMath::FloorToInt32(ResourceAS->GetMaxHealth()));
			}

			bAbilitySystemInitialized = true;

			UE_LOG(LogTemp, Log, TEXT("[%s] ASC 초기화 완료."), HasAuthority() ? TEXT("Server") : TEXT("Client"));
		}
	}
}

void AGASPlayerCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	const float NewHealth = Data.NewValue;
	UE_LOG(LogTemp, Log, TEXT("현재 체력 : %.1f"), NewHealth);

	if (Widget && Widget->GetWidget())
	{
		UDataLineWidget* HealthWidget = Cast<UDataLineWidget>(Widget->GetWidget());
		HealthWidget->UpdateName(FText::AsNumber(NewHealth));
		HealthWidget->UpdateIntValue(FMath::FloorToInt32(ResourceAS->GetMaxHealth()));
	}
}

float AGASPlayerCharacter::GetHealth() const
{
	return ResourceAS ? ResourceAS->GetHealth() : 0.0f;
}

float AGASPlayerCharacter::GetMaxHealth() const
{
	return ResourceAS ? ResourceAS->GetMaxHealth() : 0.0f;
}
