// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// Includes for various classes, so they can be referenced
#include "puzzlelaser_alertVersion.h"
#include "Interactable_Keypad.h"
#include "Interactable_Door_Single_Swing.h"

class TRIPAPROGMIKEYEDMOND_API Puzzle_Manager : public AActor
{
public:
	Puzzle_Manager();
	~Puzzle_Manager();
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	// Various functions that are used by delegates in lasers/keypads
	void Room01_UnlockDoor();
	UPROPERTY(EditAnywhere)
		AInteractable_Keypad* Room01_UnlockDoor_Keypad;
	UPROPERTY(EditAnywhere)
		AInteractable_Door_Single_Swing* Room01_UnlockDoor_Door;
};
