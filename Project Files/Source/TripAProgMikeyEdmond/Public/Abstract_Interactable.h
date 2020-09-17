// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCharacter.h"
#include "Abstract_Interactable.generated.h"

UCLASS()
class TRIPAPROGMIKEYEDMOND_API AAbstract_Interactable : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	class USceneComponent* ClassRootPoint;

public:	
	// Sets default values for this actor's properties
	AAbstract_Interactable();
	virtual void PostLoad() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*
	Abstract function to be overridden by child classes
	If not overridden, then it will display a debug message warning this
	Argument is only used by a few child, but this is easier than trying to find the player in another method
	 */
public:
	virtual void TrigInteraction(APlayerCharacter* PlayerChar);
};
