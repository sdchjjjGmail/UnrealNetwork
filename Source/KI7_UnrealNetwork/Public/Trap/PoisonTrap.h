// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoisonTrap.generated.h"

UCLASS()
class KI7_UNREALNETWORK_API APoisonTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	APoisonTrap();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void ApplyDamage();

private:
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_ActivateEffect();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trap")
	TObjectPtr<class USphereComponent> DamageVolume = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trap")
	TObjectPtr<class UNiagaraComponent> TrapEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap")
	TObjectPtr<class UNiagaraSystem> ActivateEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap")
	float DamageInterval = 0.2f;


private:
	FTimerHandle DamageTimerHandle;
	TArray<AActor*> DamageTargetActors;

};
