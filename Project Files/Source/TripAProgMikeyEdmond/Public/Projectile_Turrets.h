// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile_Abstract.h"
#include "Projectile_Turrets.generated.h"


UCLASS()
class TRIPAPROGMIKEYEDMOND_API AProjectile_Turrets : public AProjectile_Abstract
{
	GENERATED_BODY()
public:
	AProjectile_Turrets();
protected:
	// Functions for overlap
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};