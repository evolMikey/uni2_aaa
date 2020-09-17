// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile_Abstract.generated.h"

UCLASS()
class TRIPAPROGMIKEYEDMOND_API AProjectile_Abstract : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile_Abstract();
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* projMesh;
	UPROPERTY(EditAnywhere)
		class UProjectileMovementComponent* projMovement;
	UPROPERTY(EditAnywhere)
		class UBoxComponent* projCollider;

	UPROPERTY(EditAnywhere)
		float DamageValue;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
