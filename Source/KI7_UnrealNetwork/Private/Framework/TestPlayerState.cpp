// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/TestPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Characters/PlayerStateCharacter.h"

void ATestPlayerState::SetMyPlayerName(const FString& NewName)
{
	if (HasAuthority())
	{
		MyPlayerName = NewName;
		OnRep_MyDisplayName();
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

	DOREPLIFETIME(ATestPlayerState, MyPlayerName);
	DOREPLIFETIME(ATestPlayerState, MyScore);
}

void ATestPlayerState::OnRep_MyDisplayName()
{
	UE_LOG(LogTemp, Log, TEXT("Name : %s"), *MyPlayerName);
	if (APlayerStateCharacter* Character = GetPawn<APlayerStateCharacter>())
	{
		Character->UpdateNamePlate(MyPlayerName);
	}
}

void ATestPlayerState::OnRep_MyScore()
{
	UE_LOG(LogTemp, Log, TEXT("Score : %d"), MyScore);

	UE_LOG(LogTemp, Log, TEXT("Broadcast Text Set"));
	OnMyScoreChanged.Broadcast(MyScore);
}
