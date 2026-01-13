// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PracticeCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FOnLevelUpdated, int32);
DECLARE_DELEGATE_OneParam(FOnExpUpdated, float);

class UInputMappingContext;
class UInputAction;

UCLASS()
class KI7_UNREALNETWORK_API APracticeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APracticeCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnKey1();

	UFUNCTION()
	void OnKey2();

	UFUNCTION()
	void OnKey3();

private:
	UFUNCTION()
	void OnRef_Level();

	UFUNCTION()
	void OnRef_Exp();

	UFUNCTION()
	void OnRef_Health();

	void UpdateLevel();
	void UpdateExp();
	void UpdateHealth();

public:
	FOnLevelUpdated OnLevelUpdated;
	FOnExpUpdated OnExpUpdated;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UWidgetComponent> HealthWidgetComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRef_Level, Category = "Stats")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRef_Exp, Category = "Stats")
	float Exp = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRef_Health, Category = "Stats")
	float Health = 100.0f;

private:
	UPROPERTY()
	TWeakObjectPtr<class UDataLineWidget> HealthWidget = nullptr;
};
