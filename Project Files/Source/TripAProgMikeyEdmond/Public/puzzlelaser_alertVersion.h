// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "abstract_puzzlelaser.h"
#include "puzzlelaser_alertVersion.generated.h"

// Delegate for the laser, so it can trigger when walked over
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLaserTriggeredDelegate);

UCLASS()
class TRIPAPROGMIKEYEDMOND_API Apuzzlelaser_alertVersion : public Aabstract_puzzlelaser
{
	GENERATED_BODY()

public:
	// "Variable" for the delegate
	UPROPERTY(EditAnywhere, BlueprintAssignable)
		FLaserTriggeredDelegate OnLaserTrigger;

	// Overlap for this particular version of the 
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
