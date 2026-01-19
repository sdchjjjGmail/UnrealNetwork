// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/Practice/PracticeScoreActor.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Characters/PlayerStateCharacter.h"
#include "Framework/TestPlayerState.h"
#include "GameFramework/Character.h"

APracticeScoreActor::APracticeScoreActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);
	Collision->SetSphereRadius(50.f);

	Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	Effect->SetupAttachment(Collision);

    bReplicates = true;
}

void APracticeScoreActor::BeginPlay()
{
	Super::BeginPlay();
	
    if (HasAuthority()) Collision->OnComponentBeginOverlap.AddDynamic(this, &APracticeScoreActor::OnOverlap);
}

void APracticeScoreActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (HasAuthority())
    {
        ACharacter* Character = Cast<ACharacter>(OtherActor);

        if (Character)
        {
            if (ATestPlayerState* PS = Character->GetPlayerState<ATestPlayerState>())
            {
                PS->AddMyScore(10);
                Destroy();
            }
        }
    }
}
