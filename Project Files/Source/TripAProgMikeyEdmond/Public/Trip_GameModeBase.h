// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Trip_GameModeBase.generated.h"

UCLASS()
class TRIPAPROGMIKEYEDMOND_API ATrip_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATrip_GameModeBase();
	void GameOverScreen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

