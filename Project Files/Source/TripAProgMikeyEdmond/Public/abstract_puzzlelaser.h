// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "PlayerCharacter.h"
#include "abstract_puzzlelaser.generated.h"

UCLASS()
class TRIPAPROGMIKEYEDMOND_API Aabstract_puzzlelaser : public AActor
{
	GENERATED_BODY()
	UPROPERTY()
		class USceneComponent* LaserRootPoint;
	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* LaserPointStart;
	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* LaserPointEnd;
	UPROPERTY(EditDefaultsOnly)
		class UParticleSystemComponent* LaserParticles;
	UPROPERTY()
		FLinearColor LaserColour;

	// Overlap functions and component
	UPROPERTY(EditDefaultsOnly)
		class UBoxComponent* LaserBounds;
public:
	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Sets default values for this actor's properties
	Aabstract_puzzlelaser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Timer used for injuring the player regularly while they are in the overlap
	FTimerHandle DamageTimer;
	virtual void DamagePlayer();
	APlayerCharacter* PlayerCharacter;
	void SetLaserColour();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Determines if the laser damages the player/turrets or simply triggers some kind of "alert"
	UPROPERTY(EditAnywhere)
		bool bIsKiller = true;
	// Damage value per second so long as player is standing in laser beam
	UPROPERTY(EditAnywhere)
		float fDamageValue = 10;
	void ChangeDamaging(bool isNowKiller, float newDamageValue);
};
