// Fill out your copyright notice in the Description page of Project Settings.

#include "AIChar_Turret_Seeking360.h"

AAIChar_Turret_Seeking360::AAIChar_Turret_Seeking360()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAIChar_Turret_Seeking360::BeginPlay()
{
	Super::BeginPlay();
}

void AAIChar_Turret_Seeking360::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// If the turret is not currently targeting, will continuously rotate
	if (bIsIdle)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Rotating!")); DEBUGGING

		FRotator newRot = MeshHead->GetComponentRotation(); // Stores rotation of the turret for editing
		if (bRotateClockwise) // Adds or Removes from the Yaw rotation, depending on boolean
			newRot.Yaw -= fDegreesPerSecond * DeltaTime;
		else
			newRot.Yaw += fDegreesPerSecond * DeltaTime;
		MeshHead->SetRelativeRotation(newRot.Quaternion()); // Sets new rotation
	}

}