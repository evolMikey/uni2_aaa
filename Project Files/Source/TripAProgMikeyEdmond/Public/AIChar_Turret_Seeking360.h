// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIChar_Abstract_Turret.h"
#include "TimerManager.h"
#include "AIChar_Turret_Seeking360.generated.h"

UCLASS()
class TRIPAPROGMIKEYEDMOND_API AAIChar_Turret_Seeking360 : public AAIChar_Abstract_Turret
{
	GENERATED_BODY()

	FTimerHandle DetectionCooldown;

	// Does the turret head rotate clockwise or anti-clockwise?
	UPROPERTY(EditAnywhere)
	bool bRotateClockwise = true;
	// How many full rotations should the turret make per minute?
	UPROPERTY(EditAnywhere)
	float fDegreesPerSecond = true;
	// Is the turret constantly firing its weapons, or does it wait for a target?
	// Can be used for "move with firing line" puzzles
	UPROPERTY(EditAnywhere)
	bool bConstantFiring = false;
	// Does the turret track its targets, or just continue rotating when alerted?
	UPROPERTY(EditAnywhere)
	bool bTracksTargets = true;
	// If above is true, this is used to determine if its idling or not
	bool bIsIdle = true;
	UPROPERTY(EditAnywhere)
	float fVerticalAngle = 0;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	// Called when the game starts or when spawned
public:
	virtual void Tick(float DeltaTime) override; // Called every ingame tick

public:
	// Sets default values for this component's properties
	AAIChar_Turret_Seeking360();

	// Called when the turret detects a target
	void DetectedTarget();
};
