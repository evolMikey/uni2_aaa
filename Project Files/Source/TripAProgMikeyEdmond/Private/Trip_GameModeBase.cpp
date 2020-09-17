// Fill out your copyright notice in the Description page of Project Settings.

#include "Trip_GameModeBase.h"
#include "PlayerCharacter.h"

ATrip_GameModeBase::ATrip_GameModeBase()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
}

void ATrip_GameModeBase::BeginPlay()
{

}

void ATrip_GameModeBase::GameOverScreen()
{
	// Function for calling the game over functionality
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("GAME OVER")));
}