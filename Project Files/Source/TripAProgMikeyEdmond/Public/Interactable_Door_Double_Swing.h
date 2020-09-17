// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abstract_Interactable.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Components/SceneComponent.h"
#include "Interactable_Door_Double_Swing.generated.h"

UCLASS()
class TRIPAPROGMIKEYEDMOND_API AInteractable_Door_Double_Swing : public AAbstract_Interactable
{
	GENERATED_BODY()

	class USceneComponent* DoorLeftPivot;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* DoorLeftMesh;
	class USceneComponent* DoorRightPivot;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* DoorRightMesh;
	class UTimelineComponent* TimelineSwing;

public:
	// Constructor
	AInteractable_Door_Double_Swing();

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