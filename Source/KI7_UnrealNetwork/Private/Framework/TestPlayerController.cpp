// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/TestPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Characters/GASPlayerCharacter.h"

void ATestPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UEnhancedInputLocalPlayerSubsystem* SubSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (SubSystem)
	{
		SubSystem->AddMappingContext(AbilityMappingContext, 0);
	}
}

void ATestPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* Enhanced = Cast<UEnhancedInputComponent>(InputComponent);
	if (Enhanced)
	{
		Enhanced->BindAction(IA_Ability1, ETriggerEvent::Started, this, &ATestPlayerController::OnInputAbility1);
		Enhanced->BindAction(IA_Ability2, ETriggerEvent::Started, this, &ATestPlayerController::OnInputAbility2Press);
		Enhanced->BindAction(IA_Ability2, ETriggerEvent::Completed, this, &ATestPlayerController::OnInputAbility2Release);
	}
}

void ATestPlayerController::OnInputAbility1()
{
	OnAbility1Press.ExecuteIfBound();
}

void ATestPlayerController::OnInputAbility2Press()
{
	OnAbility2Press.ExecuteIfBound();
}

void ATestPlayerController::OnInputAbility2Release()
{
	OnAbility2Release.ExecuteIfBound();
}
