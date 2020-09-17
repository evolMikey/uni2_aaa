// Fill out your copyright notice in the Description page of Project Settings.

#include "puzzlelaser_alertVersion.h"

// Function called by timer when player overlaps laser, does damage to player
void Apuzzlelaser_alertVersion::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Calling Delegate-caller"));
	// IsBound checks if a function is associated with this delegate
	if (OnLaserTrigger.IsBound())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Activating Delegate!"));
		// Calls the function associated with the delegate
		OnLaserTrigger.Broadcast();
		// Ensures this is a one-time call by removing all functions
		OnLaserTrigger.Clear();
	}
}