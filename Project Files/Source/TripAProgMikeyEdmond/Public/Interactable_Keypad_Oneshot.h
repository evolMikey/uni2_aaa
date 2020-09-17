// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable_Keypad.h"
#include "Interactable_Keypad_Oneshot.generated.h"

/**
 * 
 */
UCLASS()
class TRIPAPROGMIKEYEDMOND_API AInteractable_Keypad_Oneshot : public AInteractable_Keypad
{
	GENERATED_BODY()
	
public:
	// Override for the override of the TrigInteraction function
	virtual void TrigInteraction(APlayerCharacter* PlayerChar) override;
};
