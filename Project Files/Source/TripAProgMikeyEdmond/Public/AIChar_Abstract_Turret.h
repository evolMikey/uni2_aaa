// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "AITurretGun_Component.h"
#include "TimerManager.h" // Used for time handling functions
#include "Engine/World.h" // Also used for time handling functions
#include "AIChar_Abstract_Turret.generated.h"

UCLASS()
class TRIPAPROGMIKEYEDMOND_API AAIChar_Abstract_Turret : public AActor
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AAIChar_Abstract_Turret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override; // Called every ingame tick
	void ChangeDetection(bool bNewDetection);
	void DoDamage(float fDamageVal);
	float ReturnHealth();
	FTimerHandle FireTimeHandle;
	FTimerHandle FireRepeatHandle;

protected:
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* MeshBody;
	UPROPERTY(EditAnywhere)
		class UAITurretGun_Component* TurretComponent;
	class UStaticMeshComponent* MeshHead;
	UPROPERTY(EditAnywhere)
	bool bCanDetect = true;
	float TurretHealth = 50;
	bool bTargetAcquired = false;
	
	FHitResult LineTraceFunc(float TraceDistance, ECollisionChannel TChannel, bool DebugPieces);

	void DetectedTarget();
	FVector targetLocation;
	virtual void TargetLost();

};
