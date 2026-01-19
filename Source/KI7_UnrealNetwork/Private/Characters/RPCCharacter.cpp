// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPCCharacter.h"
#include "Camera/CameraShakeBase.h"
#include "NiagaraFunctionLibrary.h"
#include "Net/UnrealNetwork.h"

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

void ARPCCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPCCharacter, Health);
}

void ARPCCharacter::Fire()
{
	if (IsLocallyControlled())	// 내가 조종하는 캐릭터다.
	{
		Server_Fire();			// 서버에게 발사 요청
	}
}

void ARPCCharacter::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (HasAuthority())
	{
		Health -= Damage;

		if (IsLocallyControlled())
		{
			OnRef_Health();	// 서버는 리플리케이션이 없으므로 수동으로 UI 같은 것들 갱신
		}

		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("맞았음"));	
		//APlayerController* PC = Cast<APlayerController>(GetController());
		//GEngine->AddOnScreenDebugMessage(
		//	-1, 5.0f, FColor::Yellow,
		//	FString::Printf(TEXT("Contoller: %s"), PC ? *PC->GetName() : TEXT("null")));
		//GEngine->AddOnScreenDebugMessage(
		//	-1, 5.0f, FColor::Yellow,
		//	FString::Printf(TEXT("Owner: %s"), GetOwner() ? *GetOwner()->GetName() : TEXT("null")));
		//GEngine->AddOnScreenDebugMessage(
		//	-1, 5.0f, FColor::Yellow,
		//	FString::Printf(TEXT("Connection: %s"), GetNetConnection() ? TEXT("O") : TEXT("X")));

		Client_OnHit();	// ClientRPC로 호출
		//Client_OnHit_Implementation(); // 그냥 호출해서 로컬 실행
	}
}

void ARPCCharacter::Server_Fire_Implementation()
{
	// 서버가 실행하는 코드
	if (ProjectileClass)
	{
		FVector SpawnLocation = FireLocation->GetComponentLocation();
		FRotator SpawnRotator = FireLocation->GetComponentRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;	// 커넥션 연결을 위해 오너 설정
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotator, SpawnParams);
	}
}

void ARPCCharacter::Client_OnHit_Implementation()
{
	FString RoleName = HasAuthority() ? TEXT("Server") : TEXT("Client");
	FString ControllerName = GetController() ? GetController()->GetName() : TEXT("NoController");

	//GEngine->AddOnScreenDebugMessage(
	//	-1, 5.0f, FColor::Red,
	//	FString::Printf(TEXT("[%s] %s : 내가 맞았음"), *RoleName, *ControllerName)
	//);

	APlayerController* PC = Cast<APlayerController>(GetController());
	PC->ClientStartCameraShake(CameraShakeClass);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(), 
		EffectClass, 
		GetActorLocation() + FVector::UpVector * 100.0f, 
		FRotator::ZeroRotator,
		FVector::OneVector, true, true, ENCPoolMethod::AutoRelease);

}

void ARPCCharacter::OnRef_Health()
{
	UE_LOG(LogTemp, Log, TEXT("체력 : %.1f"), Health);
}