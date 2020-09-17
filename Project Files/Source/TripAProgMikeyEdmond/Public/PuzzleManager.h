// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "puzzlelaser_alertVersion.h"
#include "Interactable_Keypad.h"
#include "Interactable_Keypad_Oneshot.h"
#include "Interactable_Door_Single_Swing.h"
#include "AIChar_Abstract_Turret.h"
#include "abstract_puzzlelaser.h"
#include "Runtime/Engine/Classes/Engine/TextRenderActor.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Components/TimelineComponent.h"
#include "ShootRangeTarget.h"
#include "Interactable_Pickup_Item.h"
#include "PuzzleManager.generated.h"


UCLASS()
class TRIPAPROGMIKEYEDMOND_API APuzzleManager : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	APuzzleManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Various functions that are used by delegates in lasers/keypads
	// Stuffed into regions to keep it tidy, the amount of references for some are silly
#pragma region Room 01
	UFUNCTION()
	void Room01_UnlockDoor();
	UPROPERTY(EditAnywhere, Category = "Room 01")
		class AInteractable_Keypad* Room01_UnlockDoor_Keypad;
	UPROPERTY(EditAnywhere, Category = "Room 01")
		class AInteractable_Door_Single_Swing* Room01_UnlockDoor_Door;
#pragma endregion

#pragma region Room 02
	// Functions to be binded to keypads
	UFUNCTION()
		void Room02_ActivateTrap1();
	UFUNCTION()
		void Room02_ActivateTrap2();
	UFUNCTION()
		void Room02_ActivateExit();
	// Keypads that trigger above functions
	UPROPERTY(EditAnywhere, Category = "Room 02")
		class AInteractable_Keypad* Room02_ActivateTrap_Keypad1;
	UPROPERTY(EditAnywhere, Category = "Room 02")
		class AInteractable_Keypad* Room02_ActivateTrap_Keypad2;
	UPROPERTY(EditAnywhere, Category = "Room 02")
		class AInteractable_Keypad* Room02_ActivateExit_Keypad;
	// Various actors used by the above functions
	UPROPERTY(EditAnywhere, Category = "Room 02")
		class AAIChar_Abstract_Turret* Room02_Turret1;
	UPROPERTY(EditAnywhere, Category = "Room 02")
		class AAIChar_Abstract_Turret* Room02_Turret2;
	UPROPERTY(EditAnywhere, Category = "Room 02")
		class Aabstract_puzzlelaser* Room02_Deathray1;
	UPROPERTY(EditAnywhere, Category = "Room 02")
		class Aabstract_puzzlelaser* Room02_Deathray2;
	UPROPERTY(EditAnywhere, Category = "Room 02")
		class Aabstract_puzzlelaser* Room02_Deathray3;
	UPROPERTY(EditAnywhere, Category = "Room 02")
		class Aabstract_puzzlelaser* Room02_Deathray4;
	UPROPERTY(EditAnywhere, Category = "Room 02")
		class AInteractable_Door_Single_Swing* Room02_ExitDoor;
	UPROPERTY(EditAnywhere, Category = "Room 02")
		class AInteractable_Door_Single_Swing* Room02_EnterDoor;
	UPROPERTY(EditAnywhere, Category = "Room 02")
		class ATextRenderActor* Room02_TextTimer1;
	UPROPERTY(EditAnywhere, Category = "Room 02")
		class ATextRenderActor* Room02_TextTimer2;
	// Timeline component, used to lock the door if player doesn't get up quickly enough
	UPROPERTY(EditAnywhere, Category = "Room 02")
		class UTimelineComponent* Room02_PuzzleCountdown;
	UPROPERTY(EditAnywhere, Category = "Room 02")
		UCurveFloat* Room02_Curve_Timer;
	// Delegates for update and finished functions
		FOnTimelineFloat Del_Room02_TimelineUpdate{};
		FOnTimelineEvent Del_Room02_TimelineFinished{};
	// Timeline functions, 
	UFUNCTION()
		void Room02_TimelineFloatReturn(float value); // Update function, used to set the text renderer
	UFUNCTION()
		void Room02_OnTimelineFinished(); // Finish function, used to lock the door again

#pragma endregion

#pragma region Room 03
	// Functions relating to Room 03 - Shooting Gallery
	UFUNCTION()
		void Room03_StartGame();
	UFUNCTION()
		void Room03_EndGame();
	UFUNCTION()
		void Room03_TargetDestroyed();
	UFUNCTION()
		void Room03_RemoveScoreForTime();
	int Room03_NumTargetsDestroyed = 0;
	int Room03_Score;
	FTimerHandle Room03_ScoreTimer;

	// Targets used in the Shooting Gallery
	UPROPERTY(EditAnywhere, Category = "Room 03")
		class AShootRangeTarget* Room03_Target1;
	UPROPERTY(EditAnywhere, Category = "Room 03")
		class AShootRangeTarget* Room03_Target2;
	UPROPERTY(EditAnywhere, Category = "Room 03")
		class AShootRangeTarget* Room03_Target3;
	UPROPERTY(EditAnywhere, Category = "Room 03")
		class AShootRangeTarget* Room03_Target4;
	UPROPERTY(EditAnywhere, Category = "Room 03")
		class AShootRangeTarget* Room03_Target5;
	UPROPERTY(EditAnywhere, Category = "Room 03")
		class AShootRangeTarget* Room03_Target6;
	UPROPERTY(EditAnywhere, Category = "Room 03")
		class AShootRangeTarget* Room03_Target7;
	UPROPERTY(EditAnywhere, Category = "Room 03")
		class AShootRangeTarget* Room03_Target8;
	UPROPERTY(EditAnywhere, Category = "Room 03")
		class AShootRangeTarget* Room03_Target9;
		
		// Doors in this room, locks the player in during gameplay
	UPROPERTY(EditAnywhere, Category = "Room 03")
		class AInteractable_Door_Single_Swing* Room03_EnterDoor;
	UPROPERTY(EditAnywhere, Category = "Room 03")
		class AInteractable_Door_Single_Swing* Room03_ExitDoor;

	// Keypad that kickstarts the game
	UPROPERTY(EditAnywhere, Category = "Room 03")
		class AInteractable_Keypad_Oneshot* Room03_Startgame_Keypad;
	// Text on the counter
	UPROPERTY(EditAnywhere, Category = "Room 03")
		class ATextRenderActor* Room03_ScoreKeeper;

#pragma endregion
};