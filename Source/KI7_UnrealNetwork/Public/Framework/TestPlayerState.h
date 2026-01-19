// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TestPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class KI7_UNREALNETWORK_API ATestPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddMyScore(int32 Point);

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetMyScore() const { return MyScore; }

	UFUNCTION(BlueprintCallable, Category = "Name")
	void SetMyName(const FString& NewName);

	UFUNCTION(BlueprintCallable, Category = "Name")
	const FString& GetMyName() const { return MyName; }

	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_MyScore();

	UFUNCTION()
	void OnRep_MyName();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_MyScore, BlueprintReadOnly, Category = "Data")
	int32 MyScore = 0;

	UPROPERTY(ReplicatedUsing = OnRep_MyName, BlueprintReadOnly, Category = "Data")
	FString MyName;


	TWeakObjectPtr<class UScoreHudWidget> ScoreHud = nullptr;
};
