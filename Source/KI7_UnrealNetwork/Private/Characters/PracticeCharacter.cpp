// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PracticeCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Components/WidgetComponent.h"
#include "UI/DataLineWidget.h"
#include "Framework/PracticePlayerController.h"

// Sets default values
APracticeCharacter::APracticeCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidgetComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APracticeCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(HealthWidgetComponent && HealthWidgetComponent->GetWidget())
	{
		HealthWidget = Cast<UDataLineWidget>(HealthWidgetComponent->GetWidget());
		HealthWidget->UpdateName(FText::FromString(TEXT("Health")));
	}
}

// Called every frame
void APracticeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HealthWidgetComponent)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC && PC->PlayerCameraManager)
		{
			// 회전 -> 벡터 만들기 가능(해당 회전으로 인한 Forward 백터를 만듬)
			// 벡터 -> 회전 만들기 가능

			FVector CameraForward = PC->PlayerCameraManager->GetCameraRotation().Vector();	// 카메라의 Forward 백터
			FVector WidgetForward = CameraForward * -1.0f;
			HealthWidgetComponent->SetWorldRotation(WidgetForward.Rotation());
		}
	}
}

// Called to bind functionality to input
void APracticeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APracticeCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APracticeCharacter, Health);
	DOREPLIFETIME_CONDITION(APracticeCharacter, Level, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(APracticeCharacter, Exp, COND_OwnerOnly);
}

void APracticeCharacter::OnRef_Level()
{
	UpdateLevel();
}

void APracticeCharacter::OnRef_Exp()
{
	UpdateExp();
}

void APracticeCharacter::OnRef_Health()
{
	UpdateHealth();	// 클라이언트는 리플리케이션을 받아서 업데이트
}

void APracticeCharacter::UpdateLevel()
{
	OnLevelUpdated.Execute(Level);
}

void APracticeCharacter::UpdateExp()
{
	OnExpUpdated.Execute(Exp);
}

void APracticeCharacter::UpdateHealth()
{
	if (HealthWidget.IsValid())
	{
		HealthWidget->UpdateFloatValue(Health);
	}
}

void APracticeCharacter::OnKey1()
{
	if (HasAuthority())
	{
		Level++;
		UpdateLevel();
	}
}

void APracticeCharacter::OnKey2()
{
	if (HasAuthority())
	{
		Exp += 1.0f;
		UpdateExp();
	}
}

void APracticeCharacter::OnKey3()
{
	if (HasAuthority())
	{
		Health -= 10.0f;
		UpdateHealth();	// 서버는 직접 업데이트
	}
}

