// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable_Keypad_Oneshot.h"

/*
Literally the same thing as the parent keypad, except this one removes its delegates after activating
This makes it only possible to activate once. Useful for things like loot spawning or activating enemies
*/


void AInteractable_Keypad_Oneshot::TrigInteraction(APlayerCharacter* PlayerChar)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Calling Delegate-caller!"));
	// IsBound checks if a function is associated with this delegate
	if (OnKeypadTriggered.IsBound())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Activating Delegate!"));
		// Calls the function associated with the delegate
		OnKeypadTriggered.Broadcast();
		
		// Ensures this is a one-time call by removing all functions
		OnKeypadTriggered.Clear();
	}
}