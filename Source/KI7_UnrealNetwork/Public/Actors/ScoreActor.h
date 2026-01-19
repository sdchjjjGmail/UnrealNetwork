// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreActor.generated.h"

class USphereComponent;

UCLASS()
class KI7_UNREALNETWORK_API AScoreActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AScoreActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapEvent(AActor* OverlappedActor, AActor* OtherActor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> Collision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Point = 10;

};
