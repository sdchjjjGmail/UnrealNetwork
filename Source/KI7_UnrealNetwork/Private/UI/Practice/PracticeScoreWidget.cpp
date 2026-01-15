// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Practice/PracticeScoreWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Characters/PlayerStateCharacter.h"
#include "Framework/TestPlayerState.h"

void UPracticeScoreWidget::SetMyScoreText(int32 InValue)
{
	UE_LOG(LogTemp, Log, TEXT("SetMyScoreText"));

	if (TextScore)
	{
		TextScore->SetText(FText::AsNumber(InValue));
	}
}

void UPracticeScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (InputNameText)
	{
		InputNameText->OnTextCommitted.AddDynamic(
			this,
			&UPracticeScoreWidget::OnMyNameTextCommitted
		);
	}

	SearchPlayerState();
}

void UPracticeScoreWidget::NativeDestruct()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(PlayerStateSearchHandle);
	}

	// (선택) 델리게이트도 언바인딩
	if (ATestPlayerState* PS = GetOwningPlayerState<ATestPlayerState>())
	{
		PS->OnMyScoreChanged.RemoveDynamic(this, &UPracticeScoreWidget::SetMyScoreText);
	}

	Super::NativeDestruct();
}

void UPracticeScoreWidget::OnMyNameTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod != ETextCommit::OnEnter)
		return;

	const FString NewName = Text.ToString();

	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		if (APlayerStateCharacter* MyChar = Cast<APlayerStateCharacter>(Pawn))
		{
			if (MyChar->IsLocallyControlled())
			{
				MyChar->RequestSetMyName(NewName);
			}
		}
	}
}

void UPracticeScoreWidget::SearchPlayerState()
{
	GetWorld()->GetTimerManager().SetTimer(
		PlayerStateSearchHandle,
		[this]()
		{
			if (ATestPlayerState* PS = GetOwningPlayerState<ATestPlayerState>())
			{
				UE_LOG(LogTemp, Log, TEXT("Bind Text Set"));
				PS->OnMyScoreChanged.AddUniqueDynamic(this, &UPracticeScoreWidget::SetMyScoreText);
				SetMyScoreText(PS->GetMyScore());
				GetWorld()->GetTimerManager().ClearTimer(PlayerStateSearchHandle);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Null PS"));
				if (++RetryCount > MaxRetry)
				{
					GetWorld()->GetTimerManager().ClearTimer(PlayerStateSearchHandle);
					UE_LOG(LogTemp, Warning, TEXT("Failed to bind PlayerState (timeout)"));
				}
			}
		},
		0.1f,
		true
	);
}
