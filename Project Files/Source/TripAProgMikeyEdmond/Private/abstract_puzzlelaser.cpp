// Fill out your copyright notice in the Description page of Project Settings.

#include "abstract_puzzlelaser.h"
#include "DrawDebugHelpers.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"


// Sets default values
Aabstract_puzzlelaser::Aabstract_puzzlelaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Sets up the root component point
	LaserRootPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LaserRootPoint"));

	// Sets up the start and end points
	LaserPointStart = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserPointStart"));
	LaserPointEnd = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserPointEnd"));
	LaserPointStart->SetupAttachment(RootComponent);
	LaserPointEnd->SetupAttachment(RootComponent);
	LaserPointStart->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/LaserBlock/LaserWallmount.LaserWallmount")));
	LaserPointEnd->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/LaserBlock/LaserWallmount.LaserWallmount")));


	// Sets up the laser particle systems
	LaserParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserParticles"));
	// Searches for the Laser particlesystem
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/Models/LaserBlock/PS_LaserWall.PS_LaserWall"));
	if (ParticleAsset.Succeeded())
	{
		if ((ParticleAsset.Object != NULL) || (ParticleAsset.Object != nullptr))
		{
			LaserParticles->SetTemplate(ParticleAsset.Object);
		}
	}


	// Sets up boundaries for the particle system using a box component
	LaserBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("LaserBounds"));
	// Depth and height of 4 units (roughly the same size as the particle effect)
	// Length is set to distance between the start and end meshes
	// Means if both points are moved, this box should still cover the distance
	LaserBounds->SetBoxExtent(FVector(0.2f, (FVector::Dist(LaserPointStart->RelativeLocation, LaserPointEnd->RelativeLocation)), 0.2f));

	// Sets up overlap functions
	LaserBounds->OnComponentBeginOverlap.AddDynamic(this, &Aabstract_puzzlelaser::OnOverlapBegin);
	LaserBounds->OnComponentEndOverlap.AddDynamic(this, &Aabstract_puzzlelaser::OnOverlapEnd);
}

// Called when the game starts or when spawned
void Aabstract_puzzlelaser::BeginPlay()
{
	// Hidden rotation setup
#pragma region RotationSetup
	// Depth and height of 4 units (roughly the same size as the particle effect)
	// Length is set to distance between the start and end meshes
	// Means if both points are moved, this box should still cover the distance
	LaserBounds->SetBoxExtent(FVector(((FVector::Dist(LaserPointStart->RelativeLocation, LaserPointEnd->RelativeLocation)) / 2), 4.0f, 4.0f));
	// Sets location of box to middle of both the start/end points
	LaserBounds->SetRelativeLocation(FMath::Lerp(LaserPointStart->RelativeLocation, LaserPointEnd->RelativeLocation, 0.5f));
	// Sets rotation of box to face end point (rear will face start point by nature)
	FVector DirVec;
	DirVec = LaserPointStart->GetComponentLocation() - LaserPointEnd->GetComponentLocation();
	DirVec.Normalize();
	FRotator CompRot = DirVec.Rotation();
	LaserBounds->SetWorldRotation(CompRot);
	// Sets the end point rotation using the same calculation
	LaserPointEnd->SetWorldRotation(CompRot);
	// Start point will need recalculating as DirVec needs reversing
	DirVec = LaserPointEnd->GetComponentLocation() - LaserPointStart->GetComponentLocation();
	DirVec.Normalize();
	CompRot = DirVec.Rotation();
	LaserPointStart->SetWorldRotation(CompRot);
#pragma endregion

	Aabstract_puzzlelaser::SetLaserColour();

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Starting set laser system")));
	// Was used for debugging, commenting out incase it is needed again
	//DrawDebugBox(GetWorld(), LaserBounds->GetComponentLocation(), LaserBounds->GetUnscaledBoxExtent(), LaserBounds->GetComponentRotation().Quaternion(), FColor::Purple, true, -1, 0, 10);
	Super::BeginPlay();
}

// Called every frame
void Aabstract_puzzlelaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LaserParticles->SetBeamSourcePoint(0, (LaserPointStart->GetComponentLocation()), 0);
	LaserParticles->SetBeamTargetPoint(0, (LaserPointEnd->GetComponentLocation()), 0);
}

void Aabstract_puzzlelaser::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(OtherActor))
		{
			// For debugging purposes
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Start overlap laser")));

			// Sets the player reference here to the overlapping player
			PlayerCharacter = PlayerChar;

			if (bIsKiller)
			{
				// Calls function first time so player gets hurt immediately
				Aabstract_puzzlelaser::DamagePlayer();
				// Starts timer that does damage to the player
				GetWorld()->GetTimerManager().SetTimer(DamageTimer, this, &Aabstract_puzzlelaser::DamagePlayer, 0.2f, true);
			}
		}
	}
}
void Aabstract_puzzlelaser::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(OtherActor))
		{
			// Commented out for debugging purposes
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("End overlap laser")));

			// Sets the player reference here to the overlapping player
			PlayerCharacter = nullptr;

			// Stops the timer causing damage to player
			GetWorldTimerManager().ClearTimer(DamageTimer);
		}
	}
}

// Function called by timer when player overlaps laser, does damage to player
void Aabstract_puzzlelaser::DamagePlayer()
{
	if (PlayerCharacter != nullptr)
	{
		// Debugging purposes
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Still on laser")));

		// Removes health from player
		PlayerCharacter->Health_Adjust(fDamageValue);
	}
}

void Aabstract_puzzlelaser::ChangeDamaging(bool isNowKiller, float newDamageValue)
{
	// Sets the killing boolean for the laser, as well as its damage
	bIsKiller = isNowKiller;
	fDamageValue = newDamageValue;
	// Calls the function that sets the laser colour based on whether it kills or not
	Aabstract_puzzlelaser::SetLaserColour();
}
void Aabstract_puzzlelaser::SetLaserColour()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Starting colouring laser")));
	// Sets laser colour based on whether it is designed to kill or simply alert
	if (bIsKiller)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Laser is red")));
		LaserParticles->SetColorParameter((TEXT("PSColour")), (FLinearColor().Red * 200.0f));
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Laser is blue")));
		LaserParticles->SetColorParameter((TEXT("PSColour")), (FLinearColor().Blue * 50.0f));
	}
}