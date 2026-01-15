// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerStateCharacter.generated.h"

UCLASS()
class KI7_UNREALNETWORK_API APlayerStateCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerStateCharacter();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void RequestSetMyName(const FString& NewName);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Server, Reliable)
	void ServerSetMyName(const FString& NewName);

	UFUNCTION(Server, Reliable)
	void Server_AddScore(int32 Point);

public:
	UFUNCTION(BlueprintCallable, Category = "Test")
	void TestAddScore();
};
