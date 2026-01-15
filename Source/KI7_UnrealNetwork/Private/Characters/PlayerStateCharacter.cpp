// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerStateCharacter.h"
#include "Framework/TestPlayerState.h"

// Sets default values
APlayerStateCharacter::APlayerStateCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APlayerStateCharacter::RequestSetMyName(const FString& NewName)
{
	if (!IsLocallyControlled()) return;

	ServerSetMyName(NewName);
}

// Called when the game starts or when spawned
void APlayerStateCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerStateCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerStateCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerStateCharacter::ServerSetMyName_Implementation(const FString& NewName)
{
	if (ATestPlayerState* PS = GetPlayerState<ATestPlayerState>())
	{	
		PS->SetMyPlayerName(NewName);
	}
}

void APlayerStateCharacter::Server_AddScore_Implementation(int32 Point)
{

	ATestPlayerState* PS = GetPlayerState<ATestPlayerState>();
	if (PS)
	{
		PS->AddMyScore(Point);
	}
}

void APlayerStateCharacter::TestAddScore()
{
	if (IsLocallyControlled())
	{
		Server_AddScore(10);
	}
}

