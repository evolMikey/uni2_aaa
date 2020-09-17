// Fill out your copyright notice in the Description page of Project Settings.

#include "AITurretGun_Component.h"
#include "TimerManager.h" // Used for time handling functions
#include "Engine/World.h" // Also used for time handling functions


// Sets default values for this component's properties
UAITurretGun_Component::UAITurretGun_Component()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Initializes the turret mesh
	TurretBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBody"));
	TurretBody->SetMobility(EComponentMobility::Movable);
	TurretBody->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Turrets/Import/Tur_Gun.Tur_Gun")));

	// Initalizes the muzzle-point of the turret, this is where the bullet spawns
	MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
	MuzzlePoint->SetupAttachment(TurretBody);
	MuzzlePoint->SetRelativeLocation(FVector(30.0f, 0, 20.0f));

	// Sets projectile class
	ProjectileClass = AProjectile_Turrets::StaticClass();
}

// Called when the game starts or when spawned
void UAITurretGun_Component::BeginPlay()
{
	Super::BeginPlay();
}

// Called by the parent turret when a target is found, 
void UAITurretGun_Component::FireBullet()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("FIRING!"));

	// Ensures the world is real
	UWorld* world = GetWorld();
	if (world)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FRotator SpawnRotation = TurretBody->GetComponentRotation();
		FVector SpawnLocation = MuzzlePoint->GetComponentLocation();

		world->SpawnActor<AProjectile_Turrets>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
}