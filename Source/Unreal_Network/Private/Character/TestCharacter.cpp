// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TestCharacter.h"

// Sets default values
ATestCharacter::ATestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//const FString OwnerString = GetOwner() ? GetOwner()->GetFName().ToString() : TEXT("오너 없음");
	//const FString ConnectionString = GetNetConnection() ? TEXT("커넥션 있음") : TEXT("커넥션 없음");
	//Values = FString::Printf(TEXT("Owner : %s\n Connection : %s"), *OwnerString, *ConnectionString);
	//DrawDebugString(GetWorld(), GetActorLocation(), Values, nullptr, FColor::White, 0.f, true);
}

// Called to bind functionality to input
void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATestCharacter::PrintNetworkInfo()
{
}