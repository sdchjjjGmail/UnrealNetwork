// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ScoreActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Framework/TestPlayerState.h"

AScoreActor::AScoreActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(100.0f);
	SetRootComponent(Collision);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collision);
	Mesh->SetRelativeLocation(FVector::UpVector * 100.0f);
}

void AScoreActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		OnActorBeginOverlap.AddDynamic(this, &AScoreActor::OnOverlapEvent);
	}
	
}

void AScoreActor::OnOverlapEvent(AActor* OverlappedActor, AActor* OtherActor)
{
	if (HasAuthority())
	{
		ACharacter* Character = Cast<ACharacter>(OtherActor);
		if (Character)
		{
			ATestPlayerState* PS = Character->GetPlayerState<ATestPlayerState>();
			if (PS)
			{
				PS->AddMyScore(Point);
				Destroy();
			}
		}
	}
}
