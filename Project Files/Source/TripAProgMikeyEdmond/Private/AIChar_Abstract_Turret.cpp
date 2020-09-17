// Fill out your copyright notice in the Description page of Project Settings.

#include "AIChar_Abstract_Turret.h"
#include "DrawDebugHelpers.h" // Used to draw linetrace
#include "PlayerCharacter.h"
#include "TimerManager.h" // Used for time handling functions


// Sets default values
AAIChar_Abstract_Turret::AAIChar_Abstract_Turret()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshBody"));
	MeshBody->SetupAttachment(RootComponent);
	MeshBody->SetMobility(EComponentMobility::Stationary);
	MeshBody->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Turrets/Import/Tur_BaseStatic.Tur_BaseStatic")));

	TurretComponent = CreateDefaultSubobject<UAITurretGun_Component>(TEXT("TurretComponent"));
	MeshHead = TurretComponent->TurretBody;
	MeshHead->SetupAttachment(MeshBody);
	MeshHead->SetRelativeLocation(FVector(0, 0, 65));
}

// Called when the game starts or when spawned
void AAIChar_Abstract_Turret::BeginPlay()
{
	Super::BeginPlay();
}

// Called when the game starts or when spawned
void AAIChar_Abstract_Turret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bCanDetect)
	{
		FHitResult detectScan = LineTraceFunc(5000, ECC_Pawn, false);
		if (detectScan.bBlockingHit)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("DETECTION!"));
			if (APlayerCharacter* playerChar = Cast<APlayerCharacter>(detectScan.GetActor()))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PLAYER!"));
				targetLocation = playerChar->GetActorLocation();
				DetectedTarget();
			}
		}
	}
}

// Function for lineTracing, used by the AI's target seek
FHitResult AAIChar_Abstract_Turret::LineTraceFunc(float TraceDistance, ECollisionChannel TChannel, bool DebugPieces)
{
	// Distance, output, vectors for start/end all stored
	float LocVar_TraceDistance = TraceDistance;
	FHitResult OutHit;
	FVector TraceStart = TurretComponent->MuzzlePoint->K2_GetComponentLocation();
	FVector TraceEnd = TraceStart + (TurretComponent->MuzzlePoint->GetComponentRotation().Vector() * TraceDistance);

	// Raytrace Parameters
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	// Actual Raytrace
	GetWorld()->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, TChannel, TraceParams);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor(255, 0, 0), false, 0.1f);
	if (DebugPieces)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Drawing Debugs!"));
		// Draws Raytrace line for debugging

		// Outputs if the trace hit something or not (based on its Collision Channel)
		if (OutHit.bBlockingHit)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LineTrace hit!"));
		else
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LineTrace miss!"));
	}
	return OutHit;
}

void AAIChar_Abstract_Turret::DetectedTarget()
{
	// Prevents this being called multiple times (thereby multiplying the fire-rate)
	if (!bTargetAcquired)
	{
		bTargetAcquired = true;

		// Forces the turret head to point directly at the target
		FVector DirVec;
		DirVec = targetLocation - MeshHead->GetComponentLocation();
		DirVec.Normalize();
		FRotator CompRot = DirVec.Rotation();
		CompRot = DirVec.Rotation();
		MeshHead->SetWorldRotation(CompRot);

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Target found!"));

		// Starts firing bullets
		TurretComponent->FireBullet();
		GetWorld()->GetTimerManager().SetTimer(FireRepeatHandle, this->TurretComponent, &UAITurretGun_Component::FireBullet, 0.2f, true);
		GetWorld()->GetTimerManager().SetTimer(FireTimeHandle, this, &AAIChar_Abstract_Turret::TargetLost, 3.0f, false);
	}
}
void AAIChar_Abstract_Turret::TargetLost()
{
	// Stops the timer that causes the gun to fire
	GetWorldTimerManager().ClearTimer(FireRepeatHandle);
	bTargetAcquired = false;
}

void AAIChar_Abstract_Turret::ChangeDetection(bool bNewDetection)
{
	// Detection determines whether the turret is actively seeking a target or not
	// They will still move, but will not fire at the player if this is false
	bCanDetect = bNewDetection;
}

// Function that applies damage to the target
void AAIChar_Abstract_Turret::DoDamage(float fDamageVal)
{
	TurretHealth -= fDamageVal;

	if (TurretHealth <= 0)
	{
		Destroy(this);
	}
}
// Function that returns the current health of the target
float AAIChar_Abstract_Turret::ReturnHealth()
{
	return TurretHealth;
}