// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/OwnershipTestActor.h"
#include "Characters/NetRoleCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AOwnershipTestActor::AOwnershipTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	OverlapCollision = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapCollision"));
	OverlapCollision->SetupAttachment(RootComponent);
	OverlapCollision->SetSphereRadius(400.0f);

	bReplicates = true;	// 액터의 리플리케이션 활성화
}

// Called when the game starts or when spawned
void AOwnershipTestActor::BeginPlay()
{
	Super::BeginPlay();
	//OnActorBeginOverlap.AddDynamic(this, &AOwnershipTestActor::OnOverlapped);
}

// Called every frame
void AOwnershipTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetActorLocation(), OverlapCollision->GetScaledSphereRadius(), 32, FColor::Yellow);

	if (HasAuthority())
	{
		AActor* NextOwner = nullptr;
		float MinDistance = OverlapCollision->GetScaledSphereRadius() * OverlapCollision->GetScaledSphereRadius();
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANetRoleCharacter::StaticClass(), Actors);

		for (AActor* Actor : Actors)
		{
			float Distance = GetSquaredDistanceTo(Actor);
			if (Distance <= MinDistance)
			{
				MinDistance = Distance;
				NextOwner = Actor;
			}
		}
		if (GetOwner() != NextOwner)
		{
			SetOwner(NextOwner);	// 자신에게 가장 가까운 ANetRoleCharacter를 오너로 처리
			FString OwnerName = GetOwner() ? GetOwner()->GetName() : TEXT("No Owner");
			UE_LOG(LogTemp, Log, TEXT("New Owner : %s"), *OwnerName);
		}
	}

	const FString LocalRoleString = UEnum::GetValueAsString(GetLocalRole());
	const FString RemoteRoleString = UEnum::GetValueAsString(GetRemoteRole());

	const FString OwnerString = GetOwner() ? GetOwner()->GetName() : TEXT("오너 없음");
	const FString ConnectionString = GetNetConnection() ? TEXT("커넥션 있음") : TEXT("커넥션 없음");

	const FString NetworkInfo = FString::Printf(
		TEXT("LocalRole : %s\nRemoteRole : %s\nOwner : %s\nConnection : %s"),
		*LocalRoleString, *RemoteRoleString, *OwnerString, *ConnectionString);
	DrawDebugString(GetWorld(), GetActorLocation(), NetworkInfo, nullptr, FColor::White, 0.0f, true);
}

void AOwnershipTestActor::OnOverlapped(AActor* OverlappedActor, AActor* OtherActor)
{
	if (HasAuthority())
	{
		if (GetOwner() != OtherActor)
		{
			SetOwner(OtherActor);
			FString OwnerName = GetOwner() ? GetOwner()->GetName() : TEXT("No Owner");
			UE_LOG(LogTemp, Log, TEXT("New Owner : %s"), *OwnerName);
		}
	}
}

