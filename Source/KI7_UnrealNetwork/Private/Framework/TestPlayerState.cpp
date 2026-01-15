// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/TestPlayerState.h"
#include "Net/UnrealNetwork.h"

void ATestPlayerState::SetMyPlayerName(const FString& NewName)
{
	if (HasAuthority())
	{
		MyPlayerName = NewName;
		OnRep_MyPlayerName();
	}
}

void ATestPlayerState::AddMyScore(int32 Point)
{
	if (HasAuthority())
	{
		MyScore += Point;
		OnRep_MyScore();
	}
}

void ATestPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ATestPlayerState, MyPlayerName, COND_OwnerOnly);
	DOREPLIFETIME(ATestPlayerState, MyScore);
}

void ATestPlayerState::OnRep_MyPlayerName()
{
	UE_LOG(LogTemp, Log, TEXT("Name : %s"), *MyPlayerName);
}

void ATestPlayerState::OnRep_MyScore()
{
	UE_LOG(LogTemp, Log, TEXT("Score : %d"), MyScore);

	UE_LOG(LogTemp, Log, TEXT("Broadcast Text Set"));
	OnMyScoreChanged.Broadcast(MyScore);
}
