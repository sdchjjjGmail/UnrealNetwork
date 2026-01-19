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
	// Sets default values for this character's properties
	ARPCCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintCallable, Category = "TestRPC")
	void Fire();

protected:

	// 리턴타입은 무조건 void, 커넥션이 있어야 서버로 전달이 된다.
	// 파라메터 타입은 대체로 가능(TSet, TMap 불가능)
	// 함수 구현은 뒤에 _Implementation이 붙어야 한다.
	UFUNCTION(Server, Reliable)	// Server : 서버 RPC, Reliable : 무조건 실행(수신 확인시까지 요청 반복)
	void Server_Fire();			

	UFUNCTION(Client, Reliable)
	void Client_OnHit();

	UFUNCTION()
	void OnTakeDamage(
		AActor* DamagedActor, float Damage, const class UDamageType* DamageType, 
		class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnRef_Health();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TestRPC")
	TObjectPtr<USceneComponent> FireLocation = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TestRPC")
	TSubclassOf<AActor> ProjectileClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TestRPC")
	TSubclassOf<class UCameraShakeBase> CameraShakeClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TestRPC")
	TObjectPtr<class UNiagaraSystem> EffectClass = nullptr;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRef_Health)
	float Health = 100.0f;
};
