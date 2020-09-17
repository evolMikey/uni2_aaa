// Fill out your copyright notice in the Description page of Project Settings.

#include "Abstract_Interactable.h"
#include "PlayerCharacter.h"


// Sets default values
AAbstract_Interactable::AAbstract_Interactable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Mesh object inits
	ClassRootPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ClassRootPoint"));
	RootComponent = ClassRootPoint;

	// Can't apply collisions to SceneComponents, copy the below and replace STATICMESH with the appropriate components
	//STATICMESH->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
}

void AAbstract_Interactable::PostLoad()
{
	Super::PostLoad();
}

// Called when the game starts or when spawned
void AAbstract_Interactable::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAbstract_Interactable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	 
}

void AAbstract_Interactable::TrigInteraction(APlayerCharacter* PlayerChar)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Interactable object " + (GetName()) + " does not have an override for TrigInteract")));
	// TESTING interaction against player character
}