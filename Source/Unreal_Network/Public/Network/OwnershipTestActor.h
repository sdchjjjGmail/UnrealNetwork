// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OwnershipTestActor.generated.h"

UCLASS()
class UNREAL_NETWORK_API AOwnershipTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOwnershipTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapped(AActor* OverlappedActor, AActor* OtherActor);

public:	

protected:
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;
	TObjectPtr<class USphereComponent> OverlapCollision = nullptr;
};
