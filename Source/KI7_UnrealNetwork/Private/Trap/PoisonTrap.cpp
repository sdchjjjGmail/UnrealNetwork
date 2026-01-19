// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap/PoisonTrap.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APoisonTrap::APoisonTrap()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	DamageVolume = CreateDefaultSubobject<USphereComponent>(TEXT("DamageVolue"));
	SetRootComponent(DamageVolume);

	TrapEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	TrapEffect->SetupAttachment(RootComponent);
	TrapEffect->SetAutoActivate(true);

	//ActivateEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ActivateEffect"));
	//ActivateEffect->SetupAttachment(RootComponent);
	//ActivateEffect->SetRelativeLocation(FVector::UpVector * 100.0f);
	//ActivateEffect->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void APoisonTrap::BeginPlay()
{
	Super::BeginPlay();
	
	DamageVolume->OnComponentBeginOverlap.AddDynamic(this, &APoisonTrap::OnOverlapBegin);
	DamageVolume->OnComponentEndOverlap.AddDynamic(this, &APoisonTrap::OnOverlapEnd);
}

void APoisonTrap::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() && OtherActor && OtherActor != this)
	{
		DamageTargetActors.AddUnique(OtherActor);

		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		if (!TimerManager.IsTimerActive(DamageTimerHandle))
		{
			TimerManager.SetTimer(
				DamageTimerHandle,
				this, &APoisonTrap::ApplyDamage,
				DamageInterval, true
			);
		}
	}
}

void APoisonTrap::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasAuthority() && OtherActor)
	{
		DamageTargetActors.Remove(OtherActor);
		if (DamageTargetActors.Num() <= 0)
		{
			FTimerManager& TimerManager = GetWorld()->GetTimerManager();
			TimerManager.ClearTimer(DamageTimerHandle);
		}
	}
}

void APoisonTrap::ApplyDamage()
{
	if (HasAuthority())
	{
		for (int32 i = DamageTargetActors.Num() - 1; i >= 0; --i)
		{
			AActor* Target = DamageTargetActors[i];
			if (IsValid(Target))
			{
				UGameplayStatics::ApplyDamage(Target, 1.0f, GetInstigatorController(), this, UDamageType::StaticClass());
				Multicast_ActivateEffect();
			}
			else
			{
				DamageTargetActors.RemoveAt(i);
			}
		}
	}
}

void APoisonTrap::Multicast_ActivateEffect_Implementation()
{
	if (ActivateEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ActivateEffect,
			GetActorLocation() + FVector::UpVector * 100.0f,
			GetActorRotation(),
			FVector::OneVector, true, true, 
			ENCPoolMethod::AutoRelease);
	}
}


