// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abstract_Interactable.h"
#include "EnumItemType.h"
#include "Components/StaticMeshComponent.h"
#include "Interactable_Pickup_Item.generated.h"

UCLASS()
class TRIPAPROGMIKEYEDMOND_API AInteractable_Pickup_Item : public AAbstract_Interactable
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EEnumItemType ItemType;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ItemMesh;
	UPROPERTY(EditAnywhere)
	uint8 ItemAmount;

	// Constructor
	AInteractable_Pickup_Item();

	// Trigger Interaction
	virtual void TrigInteraction(APlayerCharacter* PlayerChar) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
