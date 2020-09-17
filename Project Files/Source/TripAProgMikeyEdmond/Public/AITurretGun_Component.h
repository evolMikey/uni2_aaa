// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "Projectile_Abstract.h"
#include "Projectile_Turrets.h"
#include "AITurretGun_Component.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRIPAPROGMIKEYEDMOND_API UAITurretGun_Component : public UPrimitiveComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* TurretBody;
	class USceneComponent* MuzzlePoint;

	// Subclass for projectile
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AProjectile_Turrets> ProjectileClass;

public:	
	// Sets default values for this component's properties
	UAITurretGun_Component();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called by the turret, this will simply fire a bullet out
	void FireBullet();
};
