// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/TestGameState.h"
#include "Net/UnrealNetwork.h"

ATestGameState::ATestGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATestGameState::BeginPlay()
{
	Super::BeginPlay();
	GameRemainingTime = GameDuration;

	//PlayerArray.Num();	// 접속인원수
}

void ATestGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() && !bGameOver)
	{
		GameElapsedTime += DeltaTime;
		//UE_LOG(LogTemp, Log, TEXT("Time update : %.2f"), GameElapsedTime);

		GameRemainingTime -= DeltaTime;

		if (GameRemainingTime < 0.0f)
		{
			GameRemainingTime = 0.0f;
			bGameOver = true;
		}
	}
}

void ATestGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATestGameState, GameElapsedTime);
	DOREPLIFETIME(ATestGameState, GameRemainingTime);
	DOREPLIFETIME(ATestGameState, bGameOver);
}
