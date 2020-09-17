// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable_Door_Double_Swing.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"

AInteractable_Door_Double_Swing::AInteractable_Door_Double_Swing()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create Components
	DoorLeftPivot = CreateDefaultSubobject<USceneComponent>(TEXT("DoorLeftPivot"));
	DoorRightPivot = CreateDefaultSubobject<USceneComponent>(TEXT("DoorRightPivot"));
	DoorLeftMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorLeftMesh"));
	DoorRightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorRightMesh"));

	// Applying hierarchy and offsets
	DoorLeftPivot->SetupAttachment(RootComponent);
	DoorLeftPivot->SetMobility(EComponentMobility::Movable);
	DoorLeftPivot->SetRelativeLocation(FVector(-120.0f, 0, 0));
	DoorLeftMesh->SetupAttachment(DoorLeftPivot);
	DoorRightPivot->SetupAttachment(RootComponent);
	DoorRightPivot->SetMobility(EComponentMobility::Movable);
	DoorRightPivot->SetRelativeLocation(FVector(120.0f, 0, 0));
	DoorRightPivot->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	DoorRightMesh->SetupAttachment(DoorRightPivot);

	// Set meshes and collisions
	DoorLeftMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Doors/Import/Door_Single.Door_Single")));
	DoorRightMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Doors/Import/Door_Single.Door_Single")));
	DoorLeftMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	DoorRightMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);


	// Creates the Timeline
	TimelineSwing = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	// Binds functions
	InterpFunction.BindUFunction(this, FName("TimelineFloatReturn"));
	TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));

	fCurve = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/Curves/Curve_DoorSwing.Curve_DoorSwing"));
}

// Contains functionality for the object
void AInteractable_Door_Double_Swing::TrigInteraction(APlayerCharacter* PlayerChar)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("DOOR SWING TRIGGERED"));
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
		DoorLeftMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
		DoorRightMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	}
}

void AInteractable_Door_Double_Swing::TimelineFloatReturn(float value)
{
	// Rotates actor
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(FString::SanitizeFloat(FMath::Lerp(StartRotation, EndRotation, value).Y)));
	DoorLeftPivot->SetRelativeRotation(FRotator(0.0f, (FMath::Lerp(0.0f, 90.0f, value)), 0.0f));
	DoorRightPivot->SetRelativeRotation(FRotator(0.0f, (FMath::Lerp(-180.0f, -270.0f, value)), 0.0f));
}

void AInteractable_Door_Double_Swing::OnTimelineFinished()
{
	TimelineSwing->Reverse();
	DoorLeftMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	DoorRightMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
}