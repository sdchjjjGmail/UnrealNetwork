// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PracticeAreaDamage.generated.h"

UCLASS()
class KI7_UNREALNETWORK_API APracticeAreaDamage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APracticeAreaDamage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnEnterZone(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep, 
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnExitZone(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void GiveDamage();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<class UBoxComponent> Collision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<class UNiagaraSystem> HitEffectClass = nullptr;

private:
	TArray<AActor*> Targets;

	FTimerHandle DamageTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float DamageInterval = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float DamagePerTick = 10.f;
};
