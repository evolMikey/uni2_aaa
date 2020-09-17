// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile_Abstract.h"


// Sets default values
AProjectile_Abstract::AProjectile_Abstract()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Initializes the projectile mesh
	projMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("projMesh"));
	projMesh->SetupAttachment(RootComponent);
	projMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/msh_Projectile.msh_Projectile")));
	projMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	projMesh->SetEnableGravity(false);

	// Initializes the projectile collider to hit everything
	projCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("projCollider"));
	projCollider->SetupAttachment(projMesh);
	projCollider->SetCollisionResponseToAllChannels(ECR_Overlap);

	// Initializes the projectile movement component
	projMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("projMovement"));
	projMovement->bIsHomingProjectile = false;
	projMovement->bIsSliding = false;
	projMovement->bShouldBounce = false;
	projMovement->InitialSpeed = 5000.0f;
	projMovement->MaxSpeed = 5000.0f;
	projMovement->bRotationFollowsVelocity = true;
	projMovement->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void AProjectile_Abstract::BeginPlay()
{
	Super::BeginPlay();
}