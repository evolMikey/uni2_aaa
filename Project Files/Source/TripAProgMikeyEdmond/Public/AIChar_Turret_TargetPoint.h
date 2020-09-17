// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIChar_Abstract_Turret.h"
#include "Components/SceneComponent.h"
#include "AIChar_Turret_TargetPoint.generated.h"

/**
 * 
 */
UCLASS()
class TRIPAPROGMIKEYEDMOND_API AAIChar_Turret_TargetPoint : public AAIChar_Abstract_Turret
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		class USceneComponent* LookAtPoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this component's properties
	AAIChar_Turret_TargetPoint();
};
