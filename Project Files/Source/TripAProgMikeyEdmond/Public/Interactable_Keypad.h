// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abstract_Interactable.h"
#include "Interactable_Keypad.generated.h"

// Delegate for the keypad, so it can be given "any function"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKeypadTriggeredDelegate);

UCLASS()
class TRIPAPROGMIKEYEDMOND_API AInteractable_Keypad : public AAbstract_Interactable
{
	GENERATED_BODY()
	
public:
	AInteractable_Keypad();

	// "Variable" for the delegate
	UPROPERTY(EditAnywhere, BlueprintAssignable)
		FKeypadTriggeredDelegate OnKeypadTriggered;

	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* KeypadMesh;

	// Override for the TrigInteract function
	virtual void TrigInteraction(APlayerCharacter* PlayerChar) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
