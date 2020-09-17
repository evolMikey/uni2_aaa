// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable_Door_Single_Swing.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"

AInteractable_Door_Single_Swing::AInteractable_Door_Single_Swing()
{
	PrimaryActorTick.bCanEverTick = true;

	// Frame of door is used as a root component
	FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameMesh"));
	FrameMesh->SetupAttachment(RootComponent);
	FrameMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Doors/Frame.Frame")));
	FrameMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

	// Actual door model moves to side
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(FrameMesh);
	DoorMesh->SetMobility(EComponentMobility::Movable);
	DoorMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Doors/Door.Door")));
	DoorMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

	// Creates the Timeline
	TimelineSwing = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	// Binds functions
	InterpFunction.BindUFunction(this, FName("TimelineFloatReturn"));
	TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));

	fCurve = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/Curves/Curve_DoorSwing.Curve_DoorSwing"));
}

// Contains functionality for the object
void AInteractable_Door_Single_Swing::TrigInteraction(APlayerCharacter* PlayerChar)
{
	// Checks if door is locked, opens if possible
	if (!bIsLocked)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Door is unlocked"));
		// Super version just contains a warning message saying there is no function, so no need for Super::TrigInteraction
		if (fCurve)
		{
			// Binds float curve to the timeline
			TimelineSwing->AddInterpFloat(fCurve, InterpFunction, FName("Alpha"));
			// Binds the finish function to the timeline
			TimelineSwing->SetTimelineFinishedFunc(TimelineFinished);

			// Settings
			TimelineSwing->SetLooping(false);
			TimelineSwing->SetIgnoreTimeDilation(false);

			// Start timeline
			TimelineSwing->Play();

			// Ignores further interaction
			DoorMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
		}
	}
	else
	{
		// If door is locked, check if player has spare key and then unlock it
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Door is locked"));
	}
}

void AInteractable_Door_Single_Swing::TimelineFloatReturn(float value)
{
	DoorMesh->SetRelativeLocation(FVector(0.0f, 0.0f, (FMath::Lerp(0.0f, 300.0f, value))));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(FString::SanitizeFloat(FMath::Lerp(StartRotation, EndRotation, value).Y)));
}

void AInteractable_Door_Single_Swing::OnTimelineFinished()
{
	TimelineSwing->Reverse();
	DoorMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
}

void AInteractable_Door_Single_Swing::ChangeLockState(bool newState)
{
	bIsLocked = newState;
}