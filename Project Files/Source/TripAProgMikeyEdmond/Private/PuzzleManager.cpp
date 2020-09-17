// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleManager.h"


// Sets default values for this component's properties
APuzzleManager::APuzzleManager()
{
	// Creates the Timeline for Room02, not going to explain what each line does as this is explained in other classes
	Room02_PuzzleCountdown = CreateDefaultSubobject<UTimelineComponent>(TEXT("Room02_Curve_Timer"));
	Del_Room02_TimelineUpdate.BindUFunction(this, FName("Room02_TimelineFloatReturn"));
	Del_Room02_TimelineFinished.BindUFunction(this, FName("Room02_OnTimelineFinished"));
	Room02_Curve_Timer = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/Curves/Curve_Room02_Timer.Curve_Room02_Timer"));
	Room02_PuzzleCountdown->AddInterpFloat(Room02_Curve_Timer, Del_Room02_TimelineUpdate, FName("Alpha"));
	Room02_PuzzleCountdown->SetTimelineFinishedFunc(Del_Room02_TimelineFinished);
	Room02_PuzzleCountdown->SetLooping(false);
	Room02_PuzzleCountdown->SetIgnoreTimeDilation(false);
}


// Called when the game starts
void APuzzleManager::BeginPlay()
{
	Super::BeginPlay();
	
	// Binds the keypad in the first room to the door
	Room01_UnlockDoor_Keypad->OnKeypadTriggered.AddDynamic(this, &APuzzleManager::Room01_UnlockDoor);

	// Binds the keypad in the second room to various items
	Room02_ActivateTrap_Keypad1->OnKeypadTriggered.AddDynamic(this, &APuzzleManager::Room02_ActivateTrap1);
	Room02_ActivateTrap_Keypad2->OnKeypadTriggered.AddDynamic(this, &APuzzleManager::Room02_ActivateTrap2);
	Room02_ActivateExit_Keypad->OnKeypadTriggered.AddDynamic(this, &APuzzleManager::Room02_ActivateExit);

	// Binds keypad in the third room to a timer, binds targets to score counter
	Room03_Startgame_Keypad->OnKeypadTriggered.AddDynamic(this, &APuzzleManager::Room03_StartGame);
	// Binds targets in the third room to the scoring systems
	Room03_Target1->OnTargetDeath.AddDynamic(this, &APuzzleManager::Room03_TargetDestroyed);
	Room03_Target2->OnTargetDeath.AddDynamic(this, &APuzzleManager::Room03_TargetDestroyed);
	Room03_Target3->OnTargetDeath.AddDynamic(this, &APuzzleManager::Room03_TargetDestroyed);
	Room03_Target4->OnTargetDeath.AddDynamic(this, &APuzzleManager::Room03_TargetDestroyed);
	Room03_Target5->OnTargetDeath.AddDynamic(this, &APuzzleManager::Room03_TargetDestroyed);
	Room03_Target6->OnTargetDeath.AddDynamic(this, &APuzzleManager::Room03_TargetDestroyed);
	Room03_Target7->OnTargetDeath.AddDynamic(this, &APuzzleManager::Room03_TargetDestroyed);
	Room03_Target8->OnTargetDeath.AddDynamic(this, &APuzzleManager::Room03_TargetDestroyed);
	Room03_Target9->OnTargetDeath.AddDynamic(this, &APuzzleManager::Room03_TargetDestroyed);
}

#pragma region PUZZLE 1
void APuzzleManager::Room01_UnlockDoor()
{
	Room01_UnlockDoor_Door->ChangeLockState(false);
}
#pragma endregion


#pragma region PUZZLE 2
void APuzzleManager::Room02_ActivateTrap1()
{
	// Turrets and lasers in the room are activated
	Room02_Turret1->ChangeDetection(true);
	Room02_Turret2->ChangeDetection(true);
	Room02_EnterDoor->ChangeLockState(true);
}
void APuzzleManager::Room02_ActivateTrap2()
{
	// Lasers covering door leading back are enabled
	Room02_Deathray1->ChangeDamaging(true, 3.0f);
	Room02_Deathray2->ChangeDamaging(true, 3.0f);
	Room02_Deathray3->ChangeDamaging(true, 3.0f);
	Room02_Deathray4->ChangeDamaging(true, 3.0f);
	Room02_EnterDoor->ChangeLockState(true);
}
void APuzzleManager::Room02_ActivateExit()
{
	Room02_ExitDoor->ChangeLockState(false);
	Room02_EnterDoor->ChangeLockState(true);
	Room02_PuzzleCountdown->Play();
}
void APuzzleManager::Room02_TimelineFloatReturn(float value)
{
	Room02_TextTimer1->GetTextRender()->SetText(FText::AsNumber(value));
	Room02_TextTimer2->GetTextRender()->SetText(FText::AsNumber(value));
}
void APuzzleManager::Room02_OnTimelineFinished()
{
	Room02_ExitDoor->ChangeLockState(true);
	Room02_PuzzleCountdown->SetNewTime(0.0f);
	Room02_TextTimer1->GetTextRender()->SetText(FText::AsNumber(30.00f));
	Room02_TextTimer2->GetTextRender()->SetText(FText::AsNumber(30.00f));
}
#pragma endregion


#pragma region PUZZLE 3

void APuzzleManager::Room03_StartGame()
{
	// Called by the Keypad, starts the timer and makes all targets vulnerable, also locks player in
	// Timer that calls every half second
	GetWorld()->GetTimerManager().SetTimer(Room03_ScoreTimer, this, &APuzzleManager::Room03_RemoveScoreForTime, 0.5f, true);
	// Locks doors
	Room03_EnterDoor->ChangeLockState(true);
	Room03_ExitDoor->ChangeLockState(true);
	Room03_Target1->ChangeTargetValidity(true);
	Room03_Target2->ChangeTargetValidity(true);
	Room03_Target3->ChangeTargetValidity(true);
	Room03_Target4->ChangeTargetValidity(true);
	Room03_Target5->ChangeTargetValidity(true);
	Room03_Target6->ChangeTargetValidity(true);
	Room03_Target7->ChangeTargetValidity(true);
	Room03_Target8->ChangeTargetValidity(true);
	Room03_Target9->ChangeTargetValidity(true);
}
void APuzzleManager::Room03_EndGame()
{
	// Called when all targets are dead; stops timer, unlocks doors, drops item
	GetWorldTimerManager().ClearTimer(Room03_ScoreTimer); // Stops the timer, so score doesn't keep going down
	// Unlocks doors
	Room03_EnterDoor->ChangeLockState(false);
	Room03_ExitDoor->ChangeLockState(false);
}
void APuzzleManager::Room03_TargetDestroyed()
{
	// Called when a target is destroyed; adds points and +1 to the target destroyed int
	Room03_NumTargetsDestroyed++;
	Room03_Score += 10;
	// If all targets are destroyed, end puzzle
	if (Room03_NumTargetsDestroyed == 9)
	{
		APuzzleManager::Room03_EndGame();
	}
}
void APuzzleManager::Room03_RemoveScoreForTime()
{
	Room03_Score -= 1;
	Room03_ScoreKeeper->GetTextRender()->SetText(FText::AsNumber(Room03_Score));
}
#pragma endregion