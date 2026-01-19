// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ScoreHudWidget.h"
#include "UI/DataLineWidget.h"
#include "Components/EditableTextBox.h"
#include "Framework/TestPlayerState.h"
#include "Characters/PlayerStateCharacter.h"

void UScoreHudWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Score->UpdateName(FText::FromString(TEXT("점수")));
	Score->UpdateIntValue(0);

	NameInput->OnTextCommitted.AddDynamic(this, &UScoreHudWidget::NameCommitted);
}

void UScoreHudWidget::UpdateScore(int32 NewScore)
{
	Score->UpdateIntValue(NewScore);
}

 void UScoreHudWidget::NameCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	 UE_LOG(LogTemp, Log, TEXT("NameCommitted"));
	if (CommitMethod == ETextCommit::OnEnter)
	{
		UE_LOG(LogTemp, Log, TEXT("ETextCommit::OnEnter"));
		if (!OwningPlayerCharacter.IsValid())
		{
			if (APlayerController* PC = GetOwningPlayer())
			{
				OwningPlayerCharacter = Cast<APlayerStateCharacter>(PC->GetPawn());
			}
		}
		if (OwningPlayerCharacter.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("[%d] Name : %s"),
				OwningPlayerCharacter->GetPlayerState()->GetPlayerId(),
				*Text.ToString());
			OwningPlayerCharacter->SetMyName(Text.ToString());
		}
		UE_LOG(LogTemp, Log, TEXT("NameCommitted Finish"));
	}
}
