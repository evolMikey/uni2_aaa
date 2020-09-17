// Fill out your copyright notice in the Description page of Project Settings.

#include "ShootRangeTarget.h"


// Sets default values
AShootRangeTarget::AShootRangeTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Initializes the static mesh for this object
	// Sets attachment, gives mesh, sets scale, etc
	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
	TargetMesh->SetupAttachment(RootComponent);
	TargetMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Target/Target.Target")));
	TargetMesh->SetRelativeScale3D(FVector(5, 5, 5));
}

// Called when the game starts or when spawned
void AShootRangeTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShootRangeTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Function that applies damage to the target
void AShootRangeTarget::DoDamage(float fDamageVal)
{
	if (bIsValidTarget)
	{
		TargetHealth -= fDamageVal;

		if (TargetHealth <= 0)
		{
			// IsBound checks if a function is associated with this delegate
			if (OnTargetDeath.IsBound())
			{
				// Calls the function associated with the delegate
				OnTargetDeath.Broadcast();
				OnTargetDeath.Clear();
			}
			Destroy(this);
		}
	}
}
// Function that returns the current health of the target
float AShootRangeTarget::ReturnHealth()
{
	return TargetHealth;
}

// Function that tells the game whether this target should take damage from hits or not
void AShootRangeTarget::ChangeTargetValidity(bool isValidTarget)
{
	bIsValidTarget = isValidTarget;
}