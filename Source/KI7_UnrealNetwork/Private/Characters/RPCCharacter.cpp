// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPCCharacter.h"

// Sets default values
ARPCCharacter::ARPCCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	FireLocation = CreateDefaultSubobject<USceneComponent>(TEXT("FireLocation"));
	FireLocation->SetupAttachment(GetMesh(), TEXT("Fire"));
}

// Called when the game starts or when spawned
void ARPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	OnTakeAnyDamage.AddDynamic(this, &ARPCCharacter::OnTakeDamage);
}

// Called every frame
void ARPCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARPCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ARPCCharacter::Fire()
{
	if (IsLocallyControlled())
	{
		Server_Fire();
	}
}

void ARPCCharacter::OnTakeDamage(
	AActor* DamagedActor, 
	float Damage,
	const UDamageType* DamageType,
	AController* InstigatedBy,
	AActor* DamageCauser)
{
	if (HasAuthority())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("맞았음"));
		Client_OnHit();
	}
	//Client_OnHit_Implementation();
}

void ARPCCharacter::Server_Fire_Implementation()
{
	if (ProjectileClass)
	{
		FVector SpawnLocation = FireLocation->GetComponentLocation();
		FRotator SpawnRotation = FireLocation->GetComponentRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
}

void ARPCCharacter::Client_OnHit_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("내가 맞았음"));

	APlayerController* PC = Cast<APlayerController>(GetController());
	PC->ClientStartCameraShake(CameraShakeClass);
}
