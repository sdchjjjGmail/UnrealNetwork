// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NetworkRoll.h"

void ANetworkRoll::PrintNetworkInfo()
{
	Super::PrintNetworkInfo();
}

void ANetworkRoll::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FString LocalRoleString = UEnum::GetValueAsString(GetLocalRole());
	const FString RemoteRoleString = UEnum::GetValueAsString(GetRemoteRole());

	const FString OwnerString = GetOwner() ? GetOwner()->GetFName().ToString() : TEXT("오너 없음");
	const FString ConnectionString = GetNetConnection() ? TEXT("커넥션 있음") : TEXT("커넥션 없음");

	FString NetworkInfo =
		FString::Printf(TEXT("LocalRole : %s\n RemoteRole : %s\n Owner : %s\n Connection : %s"),
			*LocalRoleString, *RemoteRoleString, *OwnerString, *ConnectionString);
	DrawDebugString(GetWorld(), GetActorLocation(), NetworkInfo, nullptr, FColor::White, 0.f, true);
}
