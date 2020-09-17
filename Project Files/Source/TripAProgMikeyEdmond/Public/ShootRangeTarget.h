// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BillboardComponent.h"
#include "ShootRangeTarget.generated.h"

// Delegate for the keypad, so it can be given "any function"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelTargetDestroy);

UCLASS()
class TRIPAPROGMIKEYEDMOND_API AShootRangeTarget : public AActor
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* TargetMesh;

	
public:	
	// Sets default values for this actor's properties
	AShootRangeTarget();

	// "Variable" for the delegate
	UPROPERTY(EditAnywhere, BlueprintAssignable)
	FDelTargetDestroy OnTargetDeath;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	float TargetHealth = 1;
	bool bIsValidTarget = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void DoDamage(float fDamageVal);
	float ReturnHealth();
	void ChangeTargetValidity(bool isValidTarget);
};
