// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BilboardWidgetComponent.h"

UBilboardWidgetComponent::UBilboardWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetWidgetSpace(EWidgetSpace::World);
}

void UBilboardWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBilboardWidgetComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UWorld* World = GetWorld();
	if (!World) return;

	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC) return;

	APlayerCameraManager* CamMgr = PC->PlayerCameraManager;
	if (!CamMgr) return;

	const FVector CameraLocation = CamMgr->GetCameraLocation();
	const FVector WidgetLocation = GetComponentLocation();

	const FRotator LookAtRotation =
		(CameraLocation - WidgetLocation).Rotation();

	SetWorldRotation(FRotator(0.f, LookAtRotation.Yaw, 0.f));
}
