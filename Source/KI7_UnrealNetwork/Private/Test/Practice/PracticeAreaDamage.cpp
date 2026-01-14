// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/Practice/PracticeAreaDamage.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APracticeAreaDamage::APracticeAreaDamage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetBoxExtent(FVector(100.f, 100.f, 10.f));
	SetRootComponent(Collision);
}

// Called when the game starts or when spawned
void APracticeAreaDamage::BeginPlay()
{
	Super::BeginPlay();
	Collision->OnComponentBeginOverlap.AddDynamic(this, &APracticeAreaDamage::OnEnterZone);
	Collision->OnComponentEndOverlap.AddDynamic(this, &APracticeAreaDamage::OnExitZone);
}

void APracticeAreaDamage::OnEnterZone(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		if (!IsValid(OtherActor) || OtherActor == this) return;

		UE_LOG(LogTemp, Log, TEXT("Enter Zone"));

		Targets.AddUnique(OtherActor);
		if (Targets.Num() == 1)
		{
			GetWorldTimerManager().SetTimer(
				DamageTimerHandle,
				this,
				&APracticeAreaDamage::GiveDamage,
				DamageInterval,
				true,
				0.f
			);
		}
	}
}

void APracticeAreaDamage::OnExitZone(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("Exit Zone"));
		Targets.Remove(OtherActor);
		if (Targets.IsEmpty()) GetWorldTimerManager().ClearTimer(DamageTimerHandle);
	}
}

// Called every frame
void APracticeAreaDamage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APracticeAreaDamage::GiveDamage()
{
	if (!HasAuthority()) return;

	if (Targets.IsEmpty())
	{
		GetWorldTimerManager().ClearTimer(DamageTimerHandle);
		return;
	}

	if (!Targets.IsEmpty())
	{
		for (AActor* SingleTarget : Targets)
		{
			for (int32 i = Targets.Num() - 1; i >= 0; --i)
			{
				if (!IsValid(Targets[i]))
				{
					Targets.RemoveAtSwap(i);
				}
			}
			UGameplayStatics::ApplyDamage(SingleTarget, DamagePerTick, GetInstigatorController(), this, UDamageType::StaticClass());
		}
	}
}
