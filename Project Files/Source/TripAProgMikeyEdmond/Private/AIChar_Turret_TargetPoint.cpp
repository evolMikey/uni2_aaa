// Fill out your copyright notice in the Description page of Project Settings.

#include "AIChar_Turret_TargetPoint.h"

AAIChar_Turret_TargetPoint::AAIChar_Turret_TargetPoint()
{
	LookAtPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LookAtPoint"));
	LookAtPoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAIChar_Turret_TargetPoint::BeginPlay()
{
	Super::BeginPlay();
	FVector DirVec;
	// First vector should be what's being pointed out, second is current vector or something to get parallel line
	DirVec = LookAtPoint->GetComponentLocation() - MeshHead->GetComponentLocation();
	DirVec.Normalize();
	FRotator CompRot = DirVec.Rotation();
	CompRot = DirVec.Rotation();
	MeshHead->SetWorldRotation(CompRot);
}