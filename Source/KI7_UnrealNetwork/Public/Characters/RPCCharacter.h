// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPCCharacter.generated.h"

UCLASS()
class KI7_UNREALNETWORK_API ARPCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARPCCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION(BlueprintCallable, Category = "TestRPC")
	void Fire();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void Server_Fire();

	UFUNCTION(Client, Reliable)
	void Client_OnHit();

	UFUNCTION()
	void OnTakeDamage(
		AActor* DamagedActor,
		float Damage,
		const class UDamageType* DamageType,
		class AController* InstigatedBy,
		AActor* DamageCauser);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TestRPC")
	TObjectPtr<USceneComponent> FireLocation = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TestRPC")
	TSubclassOf<AActor> ProjectileClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TestRPC")
	TSubclassOf<UCameraShakeBase> CameraShakeClass = nullptr;

private:
	UPROPERTY(Replicated)
	float AccumulatedDamage = 0.f;
};
