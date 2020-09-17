// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abstract_Interactable.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Components/SceneComponent.h"
#include "Interactable_Door_Single_Swing.generated.h"

UCLASS()
class TRIPAPROGMIKEYEDMOND_API AInteractable_Door_Single_Swing : public AAbstract_Interactable
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* FrameMesh;
		class UStaticMeshComponent* DoorMesh;
		class UTimelineComponent* TimelineSwing;

public:
	// Constructor
	AInteractable_Door_Single_Swing();
	void ChangeLockState(bool newState);

	UPROPERTY(EditAnywhere)
		bool bIsLocked = false;

	// Reference to Curve, and start/end vectors
	UPROPERTY(EditAnywhere)
		UCurveFloat* fCurve;
	
	// Delegate function declaration
	FOnTimelineFloat InterpFunction{};
	FOnTimelineEvent TimelineFinished{};

	UFUNCTION() // The "Update" output from Timelines
		void TimelineFloatReturn(float value);
	UFUNCTION() // The "Finished" output from Timelines
		void OnTimelineFinished();

	virtual void TrigInteraction(APlayerCharacter* PlayerChar) override;
};