// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/TestPlayerState.h"

void ATestPlayerState::AddMyScore(int32 Point)
{
}

void ATestPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ATestPlayerState::OnRep_MyScore()
{
}
