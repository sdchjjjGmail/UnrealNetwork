// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerStateCharacter.h"
#include "Framework/TestPlayerState.h"
#include "Components/WidgetComponent.h"
#include "UI/DataLineWidget.h"

// Sets default values
APlayerStateCharacter::APlayerStateCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	NameWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("NamePlate"));
	NameWidgetComponent->SetupAttachment(GetRootComponent());
	NameWidgetComponent->SetRelativeLocation(FVector::UpVector * 105.0f);
}

// Called when the game starts or when spawned
void APlayerStateCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (NameWidgetComponent && NameWidgetComponent->GetWidget())
	{
		NameWidget = Cast<UDataLineWidget>(NameWidgetComponent->GetWidget());

		//ATestPlayerState* PS = GetPlayerState<ATestPlayerState>();
		//if (PS)
		//{
		//	NameWidget->UpdateName(FText::FromString(FString::Printf(TEXT("[%d]"), PS->GetPlayerId())));
		//}
		//else
		//{
		//	NameWidget->UpdateName(FText::FromString(TEXT("-")));
		//}
		NameWidget->UpdateName(FText::FromString(TEXT("-")));
	}
}

// Called every frame
void APlayerStateCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (NameWidgetComponent)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC && PC->PlayerCameraManager)
		{
			FVector CameraForward = PC->PlayerCameraManager->GetCameraRotation().Vector();	// 카메라의 Forward 백터
			FVector WidgetForward = CameraForward * -1.0f;
			NameWidgetComponent->SetWorldRotation(WidgetForward.Rotation());
		}
	}
}

// Called to bind functionality to input
void APlayerStateCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerStateCharacter::Server_AddScore_Implementation(int32 Point)
{
	ATestPlayerState* PS = GetPlayerState<ATestPlayerState>();
	if (PS)
	{
		PS->AddMyScore(Point);
	}
}

void APlayerStateCharacter::Server_SetMyName_Implementation(const FString& NewName)
{
	ATestPlayerState* PS = GetPlayerState<ATestPlayerState>();
	if (PS)
	{
		PS->SetMyName(NewName);
	}
}

void APlayerStateCharacter::SetMyName(const FString& NewName)
{
	if (HasAuthority())
	{
		Server_SetMyName_Implementation(NewName);
	}
	else
	{
		Server_SetMyName(NewName);
	}	
}

void APlayerStateCharacter::UpdateNamePlate(const FString& NewName)
{
	if (NameWidget.IsValid())
	{
		NameWidget->UpdateName(FText::FromString(NewName));
	}
}

void APlayerStateCharacter::TestAddScore()
{
	if (IsLocallyControlled())
	{
		Server_AddScore(10);
	}
}

