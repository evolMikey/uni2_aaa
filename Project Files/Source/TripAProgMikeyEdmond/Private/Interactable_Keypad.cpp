// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable_Keypad.h"

// Sets default values
AInteractable_Keypad::AInteractable_Keypad()
{
	KeypadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeypadMesh"));
	KeypadMesh->SetupAttachment(RootComponent);
	KeypadMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Keypad/msh_Keypad.msh_Keypad")));
	KeypadMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
}

// Called when the game starts or when spawned
void AInteractable_Keypad::BeginPlay()
{
	// Switch case sets the mesh for the item
	Super::BeginPlay();
}

void AInteractable_Keypad::TrigInteraction(APlayerCharacter* PlayerChar)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Calling Delegate-caller!"));
	// IsBound checks if a function is associated with this delegate
	if (OnKeypadTriggered.IsBound())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Activating Delegate!"));
		// Calls the function associated with the delegate
		OnKeypadTriggered.Broadcast();
	}
}