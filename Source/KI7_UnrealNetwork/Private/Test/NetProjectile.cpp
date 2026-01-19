// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/NetProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANetProjectile::ANetProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
		
	bReplicates = true;				// 이 액터는 리플리케이션을 한다.
	SetReplicateMovement(true);		// 이 액터의 무브먼트 컴포넌트는 리플리케이션이 된다.

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(20.0f);
	SetRootComponent(Collision);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetupAttachment(Collision);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	//Movement->UpdatedComponent = Collision;
	Movement->InitialSpeed = 1000.0f;
	Movement->MaxSpeed = 1000.0f;
	Movement->bShouldBounce = true;
}

// Called when the game starts or when spawned
void ANetProjectile::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &ANetProjectile::OnHit);

	if (GetInstigator())
	{
		Collision->IgnoreActorWhenMoving(GetInstigator(), true);	// 인스티게이터가 설정되어 있으면 충돌 무시하기		
	}
}

void ANetProjectile::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority())	// 서버에서만 히트 처리
	{
		if (!bHitted &&  OtherActor->IsA<ACharacter>()	// 한번도 충돌한적 없고, 캐릭터와 캐릭터의 파생클래스만 처리
			&& this != OtherActor && GetOwner() != OtherActor)	
		{
			// 데미지 주기
			UGameplayStatics::ApplyDamage(OtherActor, 10.0f, GetInstigatorController(), this, UDamageType::StaticClass());

			if (GetInstigator())
			{
				UE_LOG(LogTemp, Log, TEXT("%s가 %s를 공격했다."), *GetInstigator()->GetName(), *OtherActor->GetName());
				// 누가 누구를 공격했는지 출력
			}

			Multicast_HitEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());	// 모두에게 이팩트 재생 보이기
			SetLifeSpan(2.0f);	// 2초뒤에 삭제
			bHitted = true;		// 처음 Hit되었음을 기록
		}
	}
}

void ANetProjectile::Multicast_HitEffect_Implementation(const FVector& InLocation, const FRotator& InRotation)
{
	if(HitEffectClass)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffectClass, InLocation, InRotation);
	}
}

