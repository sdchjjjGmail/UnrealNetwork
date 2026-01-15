// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TestPlayerState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMyScoreChanged, int32, NewScore);

UCLASS()
class KI7_UNREALNETWORK_API ATestPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Player")
	void SetMyPlayerName(const FString& NewName);

	UFUNCTION(BlueprintPure, Category = "Player")
	const FString& GetMyPlayerName() const { return MyPlayerName; }

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddMyScore(int32 Point);

	UFUNCTION(BlueprintPure, Category = "Score")
	inline int32 GetMyScore() const { return MyScore; }

	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnMyScoreChanged OnMyScoreChanged;

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_MyPlayerName();

	UFUNCTION()
	void OnRep_MyScore();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_MyPlayerName, BlueprintReadOnly, Category = "Data")
	FString MyPlayerName;

	UPROPERTY(ReplicatedUsing = OnRep_MyScore, BlueprintReadOnly, Category = "Data")
	int32 MyScore = 0;

};
