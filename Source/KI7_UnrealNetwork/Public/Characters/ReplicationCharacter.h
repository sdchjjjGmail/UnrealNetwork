// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ReplicationCharacter.generated.h"

UCLASS()
class KI7_UNREALNETWORK_API AReplicationCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AReplicationCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRepNotify_Level();

public:
	UFUNCTION(BlueprintCallable)
	void TestSetLevel(int32 InLevel);

	UFUNCTION(BlueprintCallable)
	void TestSetHealth(float InHealth);

	UFUNCTION(BlueprintCallable)
	void TestSetExp(float InExp);

protected:
	UPROPERTY(ReplicatedUsing = OnRepNotify_Level)	// Level이 리플리케이션 될 때마다 OnRepNotify_Level가 실행
	int32 Level = 1;

	UPROPERTY(Replicated)	// 리플리케이션이 되지만 별도로 실행되는 함수는 없다.
	float Health = 100.0f;

	UPROPERTY(Replicated)
	float Exp = 0.0f;		


};
