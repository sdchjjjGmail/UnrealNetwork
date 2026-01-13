// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/PracticePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Characters/PracticeCharacter.h"
#include "GameFramework/PlayerState.h"
#include "UI/MainHUDWidget.h"

void APracticePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		if (HUDWidgetClass)
		{
			HUDWidget = CreateWidget<UMainHUDWidget>(this, HUDWidgetClass);
			if (HUDWidget.IsValid())
			{
				HUDWidget.Get()->AddToViewport();
			}
		}
	}
}

void APracticePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsLocalController())
	{
		UEnhancedInputLocalPlayerSubsystem* SubSystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		if (SubSystem)
		{
			SubSystem->AddMappingContext(MappingContext, 0);
		}

		UEnhancedInputComponent* EnhanedInput = Cast<UEnhancedInputComponent>(InputComponent);
		if (EnhanedInput)
		{
			EnhanedInput->BindAction(IA_Key1, ETriggerEvent::Started, this, &APracticePlayerController::OnKey1Press);
			EnhanedInput->BindAction(IA_Key2, ETriggerEvent::Started, this, &APracticePlayerController::OnKey2Press);
			EnhanedInput->BindAction(IA_Key3, ETriggerEvent::Started, this, &APracticePlayerController::OnKey3Press);
		}
	}
}

void APracticePlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	ControlledCharacter = Cast<APracticeCharacter>(aPawn);
	ControlledCharacter->OnLevelUpdated.BindWeakLambda(
		this,
		[this](int NewLevel)
		{
			if (HUDWidget.IsValid())
			{
				HUDWidget->UpdateLevel(NewLevel);
			}
		}
	);
	ControlledCharacter->OnExpUpdated.BindWeakLambda(
		this,
		[this](float NewExp)
		{
			if (HUDWidget.IsValid())
			{
				HUDWidget->UpdateExp(NewExp);
			}
		}
	);
}

void APracticePlayerController::OnUnPossess()
{
	ControlledCharacter = nullptr;
	Super::OnUnPossess();
}

void APracticePlayerController::OnKey1Press()
{
	const FString Str = FString::Printf(
		TEXT("%s[%d] : OnKey1Press"), 
		HasAuthority() ? TEXT("Server"):TEXT("Client"), 
		PlayerState->GetPlayerId());
		
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, Str);
	if (ControlledCharacter.IsValid())
	{
		ControlledCharacter.Get()->OnKey1();
	}
}

void APracticePlayerController::OnKey2Press()
{
	const FString Str = FString::Printf(
		TEXT("%s[%d] : OnKey2Press"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		PlayerState->GetPlayerId());

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, Str);
	if (ControlledCharacter.IsValid())
	{
		ControlledCharacter.Get()->OnKey2();
	}
}

void APracticePlayerController::OnKey3Press()
{
	const FString Str = FString::Printf(
		TEXT("%s[%d] : OnKey3Press"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		PlayerState->GetPlayerId());

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, Str);
	if (ControlledCharacter.IsValid())
	{
		ControlledCharacter.Get()->OnKey3();
	}
}