// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/NetProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANetProjectile::ANetProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(true);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(20.f);
	SetRootComponent(Collision);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetupAttachment(Collision);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	//Movement->UpdatedComponent = Collision;
	Movement->InitialSpeed = 3000.f;
	Movement->MaxSpeed = 3000.f;
	Movement->bShouldBounce = true;
}

// Called when the game starts or when spawned
void ANetProjectile::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &ANetProjectile::OnHit);

	if (GetInstigator())
	{
		Collision->IgnoreActorWhenMoving(GetInstigator(), true);
	}
}

void ANetProjectile::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority())
	{
		if (!bHit && OtherActor->IsA<ACharacter>())
		{
			UGameplayStatics::ApplyDamage(OtherActor, 10.f, GetInstigatorController(), this, UDamageType::StaticClass());

			if (GetInstigator()) UE_LOG(LogTemp, Log, TEXT("%s가 %s를 공격했다."), *GetInstigator()->GetName(), *OtherActor->GetName());

			Multicast_HitEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			SetLifeSpan(2.f);
			bHit = true;
		}
	}
}

void ANetProjectile::Multicast_HitEffect_Implementation(const FVector& InLocation, const FRotator& InRotation)
{
	if (HitEffectClass)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffectClass, InLocation, InRotation);
	}
}

