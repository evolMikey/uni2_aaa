 // Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h" // Used to draw linetrace
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Abstract_Interactable.h"
#include "Interactable_Pickup_Item.h"
#include "InventoryObject.h"
#include "Blueprint/UserWidget.h"
#include "Trip_GameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include <UObjectGlobals.h>

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Camera component
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PlayerCamera->SetupAttachment(GetRootComponent());
	PlayerCamera->bUsePawnControlRotation = true; // For some reason Y-axis turning doesn't work without this
												  // Interaction trigger-sphere
	Interact_Radius = CreateDefaultSubobject<USphereComponent>(TEXT("Interact_Radius"));
	Interact_Radius->InitSphereRadius(PVar_TriggerRadius);
	Interact_Radius->SetupAttachment(GetRootComponent());
	Interact_Radius->SetCollisionProfileName("Trigger");
	Interact_Radius->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	Interact_Radius->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);
	Interact_Radius->SetHiddenInGame(true); // Makes bounds of shape components visible

	// Character movement defaults
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->JumpZVelocity = PVar_JumpHeight;

	// Initializes all the defaults associated with the mesh
	PlayerHeldGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerHeldGun"));
	PlayerHeldGun->SetupAttachment(PlayerCamera);
	PlayerHeldGun->SetRelativeLocation(FVector(30.0f, 25.0f, -25.0f));
	PlayerHeldGun->SetRelativeRotation(FRotator(0, 110, 0));
	PlayerHeldGun->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static")));
	PlayerHeldGun->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	PlayerHeldGun->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	PlayerHeldGun->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	PlayerHeldGun->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
	PlayerHeldGun->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	wPlayerHUD = LoadClass<UUserWidget>(NULL, (TEXT("/Game/UI/HUD_Main.HUD_Main_C")));

	// Sets projectile class
	ProjectileClass = AProjectile_Player::StaticClass();
}

void PostInitializeComponents()
{
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Adds HUD to the player
	if (wPlayerHUD)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Widget exists!"));
		PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), wPlayerHUD);
		PlayerHUDWidget->AddToViewport();
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (PVar_IsSeekingInteract)
	{
		FHitResult LocVar_hitReturn = LineTraceFunc(180, ECC_GameTraceChannel1, false);
		if (LocVar_hitReturn.bBlockingHit)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("HIT!")));
			// Casts the hit object against the Interactable class, disregards if failed
			if (AAbstract_Interactable* ScanObject = Cast<AAbstract_Interactable>(LocVar_hitReturn.GetActor()))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("SUCCEED!")));
				PVar_InteractObject = ScanObject;
				PVar_HasFoundObject = true;
			}
			else // If somehow a non-Interactable is found on Trace-Channel 1, it will still return nullptr
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("FAILED!")));
				PVar_InteractObject = nullptr;
				PVar_HasFoundObject = false;
			}
		}
		else // If linetrace finds no target, removes pointer reference (prevents player from entering radius, looking at object, looking away, and still being able to interact)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("MISS!")));
			PVar_InteractObject = nullptr;
			PVar_HasFoundObject = false;
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Character movement binds
	InputComponent->BindAxis("Input_MoveForward", this, &APlayerCharacter::Control_Movement_Forward);
	InputComponent->BindAxis("Input_MoveSide", this, &APlayerCharacter::Control_Movement_Side);
	InputComponent->BindAxis("Input_LookHorizontal", this, &APlayerCharacter::AddControllerYawInput);
	InputComponent->BindAxis("Input_LookVertical", this, &APlayerCharacter::AddControllerPitchInput);
	InputComponent->BindAction("Input_Jump", IE_Pressed, this, &APlayerCharacter::Control_Jump);
	InputComponent->BindAction("Input_Crouch", IE_Pressed, this, &APlayerCharacter::Control_Crouch);

	// Combat binds
	InputComponent->BindAction("Input_Fire", IE_Pressed, this, &APlayerCharacter::Control_FireWeapon);
	InputComponent->BindAction("Input_Reload", IE_Pressed, this, &APlayerCharacter::Control_ReloadWeapon);
	InputComponent->BindAction("Input_ADS", IE_Pressed, this, &APlayerCharacter::Control_AimDownSights);
	InputComponent->BindAction("Input_Melee", IE_Pressed, this, &APlayerCharacter::Control_MeleeAttack);

	// Generic binds
	InputComponent->BindAction("Input_Interact", IE_Pressed, this, &APlayerCharacter::Control_Interact);

	InputComponent->BindAction("input_Weap1", IE_Pressed, this, &APlayerCharacter::Control_Weapon1);
	InputComponent->BindAction("input_Weap2", IE_Pressed, this, &APlayerCharacter::Control_Weapon2);
	InputComponent->BindAction("Input_Item1", IE_Pressed, this, &APlayerCharacter::Control_Item1);
	InputComponent->BindAction("Input_Item2", IE_Pressed, this, &APlayerCharacter::Control_Item2);
	InputComponent->BindAction("Input_Item3", IE_Pressed, this, &APlayerCharacter::Control_Item3);
	InputComponent->BindAction("Input_Grenade", IE_Pressed, this, &APlayerCharacter::Control_Item4);
}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Detects beginning of collisions
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Player began overlap object")));
		// Overlapping with Interactable Object
		if (AAbstract_Interactable* InteractObject = Cast<AAbstract_Interactable>(OtherActor))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Overlapping Interactable")));
			PVar_IsSeekingInteract = true; // Start linetracing
		}
	}
}
void APlayerCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Detects end of collisions
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Player end overlap object")));
		// Overlapping with Interactable Object
		if (AAbstract_Interactable* InteractObject = Cast<AAbstract_Interactable>(OtherActor))
		{
			PVar_IsSeekingInteract = false; // Stop linetracing
			PVar_InteractObject = nullptr; // Removes currently "selected" interactable
			PVar_HasFoundObject = false;
		}
	}
}

// Functions for player movement/turning, names should be self-explanatory
void APlayerCharacter::Control_Movement_Forward(float AxisInput)
{
	// Temporary float to store movement speed
	float LocVar_Speed = PVar_Speed * AxisInput;

	// Stores the character rotation as a Vector, disregarding Pitch
	FRotator charRotation = GetControlRotation();
	charRotation.Pitch = 0;
	FVector MovementDirection = FRotationMatrix(charRotation).GetScaledAxis(EAxis::X);

	// If crouching, move at 2/3 the usual speed
	if (PVar_Crouching)
		LocVar_Speed = ((PVar_Speed / 50) * 2) * AxisInput;

	// Moves the player along given vector with input value
	AddMovementInput(MovementDirection, LocVar_Speed);
}
void APlayerCharacter::Control_Movement_Side(float AxisInput)
{
	// Temporary float to store movement speed
	// Sideways movement is 4/5th the normal
	float LocVar_Speed = PVar_Speed * AxisInput;

	// Stores the character rotation as a Vector, disregarding Pitch
	FRotator charRotation = GetControlRotation();
	charRotation.Pitch = 0;
	FVector MovementDirection = FRotationMatrix(charRotation).GetScaledAxis(EAxis::Y);

	// If crouching, move at 2/3 the usual speed
	if (PVar_Crouching)
		LocVar_Speed = (PVar_Speed / 50) * AxisInput;

	// Moves the player along given vector with input value
	AddMovementInput(MovementDirection, LocVar_Speed);
}
void APlayerCharacter::Control_Crouch()
{
	if (PVar_Crouching)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Now standing!"));
		PVar_Crouching = false;
		UnCrouch();
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Now crouching!"));
		PVar_Crouching = true;
		Crouch();
	}
}
void APlayerCharacter::Control_Jump()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Jumping!"));
	Jump();
}

// Functions for combat
void APlayerCharacter::Control_FireWeapon()
{
	// First checks if the current equip gun is actually a gun
	if ((PVar_EquipGun1) && (PVar_Weapon1 == EEnumItemType::ITEM_EMPTY))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Gun 1 is empty hand!"));
		return;
	}
	if ((!PVar_EquipGun1) && (PVar_Weapon2 == EEnumItemType::ITEM_EMPTY))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Gun 2 is empty hand!"));
		return;
	}

	// Checks if gun 1 or 2 is equip, this is used for ammo counting
	if (PVar_EquipGun1)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Gun 1!"));
		// Checks if gun 1 has ammo, if so remove one bullet, else return out of function
		if (PVar_Weap1Ammo > 0)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Removing ammo!"));
			PVar_Weap1Ammo--;
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Out of ammo!"));
			return;
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Gun 2!"));
		// Checks if gun 2 has ammo, if so remove one bullet, else return out of function
		if (PVar_Weap2Ammo > 0)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Removing ammo!"));
			PVar_Weap2Ammo--;
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Out of ammo!"));
			return;
		}
	}

	// Returns in the "no ammo" blocks would prevent this ever being reached
	// Ammo has already been deducted, so fire away!

	// First up! Pistols!
	// If statement in English:
	// If gun 1 is held and is a pistol OR gun 2 is held and is a pistol...
	if (((PVar_EquipGun1) && (PVar_Weapon1 == EEnumItemType::ITEM_WEAP_Pistol)) || ((!PVar_EquipGun1) && (PVar_Weapon2 == EEnumItemType::ITEM_WEAP_Pistol)))
	{
		// Ensures the world is real
		UWorld* world = GetWorld();
		if (world)
		{
			// Bullet will always spawn
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			// Sets rotation and location of bullet on spawn
			FRotator SpawnRotation = PlayerCamera->GetComponentRotation();
			FVector SpawnLocation = PlayerHeldGun->GetComponentLocation();
			// Spawns bullet in
			world->SpawnActor<AProjectile_Player>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
		}
	}

	// Second up! Shotguns!
	// If statement similar to pistols, but with more options:
	// If gun 1 is held and is a shotgun-standard or shotgun-shock or shotgun-explosive...
	if (((PVar_EquipGun1) && (PVar_Weapon1 == EEnumItemType::ITEM_WEAP_ShotgunStandard)) || ((!PVar_EquipGun1) && (PVar_Weapon2 == EEnumItemType::ITEM_WEAP_ShotgunStandard)))
	{
		// Ensures the world is real
		UWorld* world = GetWorld();
		// For-loop, so that 5 bullets spawn instead of 1
		for (size_t i = 0; i < 5; i++)
		{
			if (world)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SHOTGUN!"));
				// Bullet will always spawn
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				// Sets rotation and location of bullet on spawn
				// Rotation is given an additional 20-degree range on all axis, for spread effect
				FRotator SpawnRotation = (PlayerCamera->GetComponentRotation() + FRotator(FMath::RandRange(-10.0f, 10.0f), FMath::RandRange(-10.0f, 10.0f), FMath::RandRange(-10.0f, 10.0f)));
				FVector SpawnLocation = PlayerHeldGun->GetComponentLocation();
				// Spawns bullet in
				world->SpawnActor<AProjectile_Player>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
			}
		}
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Firing main cannon!"));
	// Does a Linetrace to detect specific channel at specific range
	FHitResult LocVar_hitReturn = LineTraceFunc(PVar_WeaponRange, ECC_Visibility, true);
	// If Linetrace returns true on given channel, do things
	// hitReturn only returns true on shootable objects, how they react depends on class
	if (LocVar_hitReturn.Actor != NULL)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, LocVar_hitReturn.Actor->GetName());
		if (AAIChar_Abstract_Turret* EnemyTarget = Cast<AAIChar_Abstract_Turret>(LocVar_hitReturn.GetActor()))
		{
			EnemyTarget->DoDamage(APlayerCharacter::PVar_WeaponDamage);
		}
		else if (AShootRangeTarget* EnemyTarget = Cast<AShootRangeTarget>(LocVar_hitReturn.GetActor()))
		{
			EnemyTarget->DoDamage(APlayerCharacter::PVar_WeaponDamage);
		}
	}
}
void APlayerCharacter::Control_ReloadWeapon()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Attempting reload"));
	if (!bIsReloading)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Can reload"));
		bIsReloading = true;
		// Checks if first or second weapon is held
		if (PVar_EquipGun1)
		{
			// Switch case between all available weapons
			switch (PVar_Weapon1)
			{
				// Pistols
			case EEnumItemType::ITEM_WEAP_Pistol:
				// Returns mag to ammo counter
				PVar_PistolAmmo = PVar_PistolAmmo + PVar_Weap1Ammo;
				// Checks if there is enough ammo for a full mag
				if (PVar_PistolAmmo >= 9)
				{
					// Loads full clip, removes clip from reserves
					PVar_Weap1Ammo = 9;
					PVar_PistolAmmo = PVar_PistolAmmo - 9;
				}
				else
				{
					// Loads as much as possible into gun, reserves at 0
					PVar_Weap1Ammo = PVar_PistolAmmo;
					PVar_PistolAmmo = 0;
				}
				break;
				// Rifles
			case EEnumItemType::ITEM_WEAP_RifleExplosive:
			case EEnumItemType::ITEM_WEAP_RifleStandard:
			case EEnumItemType::ITEM_WEAP_RifleShock:
				// Returns mag to ammo counter
				PVar_RifleAmmo = PVar_RifleAmmo + PVar_Weap1Ammo;
				// Checks if there is enough ammo for a full mag
				if (PVar_RifleAmmo >= 30)
				{
					// Loads full clip, removes clip from reserves
					PVar_Weap1Ammo = 30;
					PVar_RifleAmmo = PVar_RifleAmmo - 30;
				}
				else
				{
					// Loads as much as possible into gun, reserves at 0
					PVar_Weap1Ammo = PVar_RifleAmmo;
					PVar_RifleAmmo = 0;
				}
				break;
				// Shotguns
			case EEnumItemType::ITEM_WEAP_ShotgunExplosive:
			case EEnumItemType::ITEM_WEAP_ShotgunStandard:
			case EEnumItemType::ITEM_WEAP_ShotgunShock:
				// Returns mag to ammo counter
				PVar_ShotgunAmmo = PVar_ShotgunAmmo + PVar_Weap1Ammo;
				// Checks if there is enough ammo for a full mag
				if (PVar_ShotgunAmmo >= 5)
				{
					// Loads full clip, removes clip from reserves
					PVar_Weap1Ammo = 5;
					PVar_ShotgunAmmo = PVar_ShotgunAmmo - 5;
				}
				else
				{
					// Loads as much as possible into gun, reserves at 0
					PVar_Weap1Ammo = PVar_ShotgunAmmo;
					PVar_ShotgunAmmo = 0;
				}
				break;
			}
		}
		else
		{
			// Switch case between all available weapons
			switch (PVar_Weapon2)
			{
				// Pistols
			case EEnumItemType::ITEM_WEAP_Pistol:
				// Returns mag to ammo counter
				PVar_PistolAmmo = PVar_PistolAmmo + PVar_Weap2Ammo;
				// Checks if there is enough ammo for a full mag
				if (PVar_PistolAmmo >= 9)
				{
					// Loads full clip, removes clip from reserves
					PVar_Weap2Ammo = 9;
					PVar_PistolAmmo = PVar_PistolAmmo - 9;
				}
				else
				{
					// Loads as much as possible into gun, reserves at 0
					PVar_Weap2Ammo = PVar_PistolAmmo;
					PVar_PistolAmmo = 0;
				}
				break;
				// Rifles
			case EEnumItemType::ITEM_WEAP_RifleExplosive:
			case EEnumItemType::ITEM_WEAP_RifleStandard:
			case EEnumItemType::ITEM_WEAP_RifleShock:
				// Returns mag to ammo counter
				PVar_RifleAmmo = PVar_RifleAmmo + PVar_Weap2Ammo;
				// Checks if there is enough ammo for a full mag
				if (PVar_RifleAmmo >= 30)
				{
					// Loads full clip, removes clip from reserves
					PVar_Weap2Ammo = 30;
					PVar_RifleAmmo = PVar_RifleAmmo - 30;
				}
				else
				{
					// Loads as much as possible into gun, reserves at 0
					PVar_Weap2Ammo = PVar_RifleAmmo;
					PVar_RifleAmmo = 0;
				}
				break;
				// Shotguns
			case EEnumItemType::ITEM_WEAP_ShotgunExplosive:
			case EEnumItemType::ITEM_WEAP_ShotgunStandard:
			case EEnumItemType::ITEM_WEAP_ShotgunShock:
				// Returns mag to ammo counter
				PVar_ShotgunAmmo = PVar_ShotgunAmmo + PVar_Weap2Ammo;
				// Checks if there is enough ammo for a full mag
				if (PVar_ShotgunAmmo >= 5)
				{
					// Loads full clip, removes clip from reserves
					PVar_Weap2Ammo = 5;
					PVar_ShotgunAmmo = PVar_ShotgunAmmo - 5;
				}
				else
				{
					// Loads as much as possible into gun, reserves at 0
					PVar_Weap2Ammo = PVar_ShotgunAmmo;
					PVar_ShotgunAmmo = 0;
				}
				break;
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Doing reload"));
		// Starts 5 second timer that prevents rapid-reloading or firing of guns
		GetWorld()->GetTimerManager().SetTimer(TimerReload, this, &APlayerCharacter::ReloadWaitTime, 5.0f, false);
	}
}
void APlayerCharacter::ReloadWaitTime()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Finished reloading"));
	// This function is called by the Timer to allow the player to fire/reload again after reloading a weapon
	bIsReloading = false;
}
void APlayerCharacter::Control_AimDownSights()
{
}
void APlayerCharacter::Control_MeleeAttack()
{
	// Does a Linetrace to detect specific channel at specific range
	FHitResult LocVar_hitReturn = LineTraceFunc(100, ECC_Visibility, false);
	// If Linetrace returns true on given channel, do things
	// hitReturn only returns true on meleeable objects, how they react depends on class
	if (LocVar_hitReturn.Actor != NULL)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, LocVar_hitReturn.Actor->GetName());
	}
}

void APlayerCharacter::Control_Item1()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Item 1!"));
}
void APlayerCharacter::Control_Item2()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Item 2!"));
}
void APlayerCharacter::Control_Item3()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Item 3!"));
}
void APlayerCharacter::Control_Item4()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Item 4!"));
}
void APlayerCharacter::Control_Weapon1()
{
	PVar_EquipGun1 = true;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Weapon 1!"));
	switch (PVar_Weapon1)
	{
	case EEnumItemType::ITEM_WEAP_Pistol:
		PlayerHeldGun->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static")));
		PVar_WeaponDamage = 30;
		PVar_WeaponRange = 1500;
		break;
	case EEnumItemType::ITEM_WEAP_RifleStandard:
	case EEnumItemType::ITEM_WEAP_RifleShock:
	case EEnumItemType::ITEM_WEAP_RifleExplosive:
		PlayerHeldGun->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/M4A1_Static.M4A1_Static")));
		PVar_WeaponDamage = 40;
		PVar_WeaponRange = 2500;
		break;
	case EEnumItemType::ITEM_WEAP_ShotgunStandard:
	case EEnumItemType::ITEM_WEAP_ShotgunShock:
	case EEnumItemType::ITEM_WEAP_ShotgunExplosive:
		PlayerHeldGun->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/Mossberg_Static.Mossberg_Static")));
		PVar_WeaponDamage = 70;
		PVar_WeaponRange = 1000;
		break;
	}
	PlayerHeldGun->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	PlayerHeldGun->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	PlayerHeldGun->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	PlayerHeldGun->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
	PlayerHeldGun->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}
void APlayerCharacter::Control_Weapon2()
{
	PVar_EquipGun1 = false;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Weapon 2!"));
	switch (PVar_Weapon2)
	{
	case EEnumItemType::ITEM_WEAP_Pistol:
		PlayerHeldGun->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static")));
		PVar_WeaponDamage = 30;
		PVar_WeaponRange = 1500;
		break;
	case EEnumItemType::ITEM_WEAP_RifleStandard:
	case EEnumItemType::ITEM_WEAP_RifleShock:
	case EEnumItemType::ITEM_WEAP_RifleExplosive:
		PlayerHeldGun->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/M4A1_Static.M4A1_Static")));
		PVar_WeaponDamage = 40;
		PVar_WeaponRange = 2500;
		break;
	case EEnumItemType::ITEM_WEAP_ShotgunStandard:
	case EEnumItemType::ITEM_WEAP_ShotgunShock:
	case EEnumItemType::ITEM_WEAP_ShotgunExplosive:
		PlayerHeldGun->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/Mossberg_Static.Mossberg_Static")));
		PVar_WeaponDamage = 70;
		PVar_WeaponRange = 1000;
		break;
	}
	PlayerHeldGun->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	PlayerHeldGun->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	PlayerHeldGun->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	PlayerHeldGun->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
	PlayerHeldGun->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

// Function for lineTracing
FHitResult APlayerCharacter::LineTraceFunc(float TraceDistance, ECollisionChannel TChannel, bool DebugPieces)
{
	// Distance, output, vectors for start/end all stored
	float LocVar_TraceDistance = TraceDistance;
	FHitResult OutHit;
	FVector TraceStart = PlayerCamera->GetComponentLocation();
	FVector TraceEnd = TraceStart + (PlayerCamera->GetComponentRotation().Vector() * TraceDistance);

	// Raytrace Parameters
	FCollisionQueryParams TraceParams;

	// Actual Raytrace
	GetWorld()->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, TChannel, TraceParams);
	if (DebugPieces)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Drawing Debugs!"));
		// Draws Raytrace line for debugging
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor(255, 0, 0), false, 1.5f);

		// Outputs if the trace hit something or not (based on its Collision Channel)
		if (OutHit.bBlockingHit)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LineTrace hit!"));
		else
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LineTrace miss!"));
	}
	return OutHit;
}

void APlayerCharacter::Control_Interact()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("INTERACTING!")));
	// Checks the object is neither null, nullptr, and IS an Interactable
	// Probably quite redundant to have multiple checks, but I don't want to accidentally create a blackhole
	if ((PVar_InteractObject != nullptr) && (PVar_InteractObject != NULL))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Object highlighted!")));
		if (AAbstract_Interactable* InterObject = Cast<AAbstract_Interactable>(PVar_InteractObject))
		{
			// Debug message
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Object is Interactable!")));
			// Sends message out to object in question
			InterObject->TrigInteraction(this);
		}
	}
	//else // Debug message
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("No object highlighted!")));
}

// Functions relating to inventory
void APlayerCharacter::Inv_PickupItem(EEnumItemType ItemPickup, int ItemAmount)
{
	/*
	Switch case that covers every item in the EEnumItemType list, anything else (default) just adds scrap metal
	Prints out what is being picked up (for debugging purposes), flashes up the appropriate hud icon,
	In the case of weapons, it will swap whatever is currently being carried for the picked up gun
	*/
	switch (ItemPickup)
	{
		// Pistol ammo
	case EEnumItemType::ITEM_AMMO_Pistol:
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Picked up pistol ammo")));
		APlayerCharacter::PVar_PistolAmmo += ItemAmount;
		break;
		// Rifle ammo
	case EEnumItemType::ITEM_AMMO_Rifle:
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Picked up rifle ammo")));
		APlayerCharacter::PVar_RifleAmmo += ItemAmount;
		break;
		// Shotgun ammo
	case EEnumItemType::ITEM_AMMO_Shotgun:
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Picked up shotgun ammo")));
		APlayerCharacter::PVar_ShotgunAmmo += ItemAmount;
		break;
		// Frag grenades
	case EEnumItemType::ITEM_WEAP_GrenadeExplosive:
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Picked up frag grenades")));
		APlayerCharacter::PVar_GrenadeFrag += ItemAmount;
		break;
		// EMP grenades
	case EEnumItemType::ITEM_WEAP_GrenadeShock:
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Picked up EMP grenades")));
		APlayerCharacter::PVar_GrenadeEmp += ItemAmount;
		break;
		// Adhesive grenades
	case EEnumItemType::ITEM_WEAP_GrenadeSticky:
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Picked up adhesive grenades")));
		APlayerCharacter::PVar_GrenadeStick += ItemAmount;
		break;
		// Healthpacks
	case EEnumItemType::ITEM_PICK_Healthpack:
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Picked up a healthpack")));
		APlayerCharacter::PVar_Healthpack += ItemAmount;
		break;
		// Camo modules
	case EEnumItemType::ITEM_PICK_Camo:
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Picked up a camo module")));
		APlayerCharacter::PVar_Camo += ItemAmount;
		break;
		// Shielding moduules
	case EEnumItemType::ITEM_PICK_Shielding:
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Picked up a shield module")));
		APlayerCharacter::PVar_Shield += ItemAmount;
		break;
		// Keycards
	case EEnumItemType::ITEM_PICK_Keycard:
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Picked up a keycard")));
		APlayerCharacter::PVar_Keycard += ItemAmount;
		break;
		// Scrap metal
	case EEnumItemType::ITEM_PICK_ScrapMetal:
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Picked up scrap metal")));
		APlayerCharacter::PVar_ScrapMetal += ItemAmount;
		break;

		// Weapon pickups
	case EEnumItemType::ITEM_WEAP_Pistol:
	case EEnumItemType::ITEM_WEAP_RifleExplosive:
	case EEnumItemType::ITEM_WEAP_RifleShock:
	case EEnumItemType::ITEM_WEAP_RifleStandard:
	case EEnumItemType::ITEM_WEAP_ShotgunExplosive:
	case EEnumItemType::ITEM_WEAP_ShotgunShock:
	case EEnumItemType::ITEM_WEAP_ShotgunStandard:
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Picked up new weapon")));
		// Are either of their weapon slots empty?
		if (APlayerCharacter::PVar_Weapon1 == EEnumItemType::ITEM_EMPTY)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Slot 1 is empty")));
			PVar_Weapon1 = ItemPickup;
			APlayerCharacter::Control_Weapon1();
		}
		else if (APlayerCharacter::PVar_Weapon2 == EEnumItemType::ITEM_EMPTY)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Slot 2 is empty")));
			PVar_Weapon2 = ItemPickup;
			APlayerCharacter::Control_Weapon2();
		}
		// If neither slot is empty, drop currently held gun and replace
		else if (APlayerCharacter::PVar_EquipGun1)
		{
			APlayerCharacter::Inv_DropItem(PVar_Weapon1);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Swap 1 is replaced")));
			PVar_Weapon1 = ItemPickup;
			APlayerCharacter::Control_Weapon1();
		}
		else // If equpgun1 is false, that means second slot is equip
		{
			APlayerCharacter::Inv_DropItem(PVar_Weapon2);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Slot 2 is replaced")));
			PVar_Weapon2 = ItemPickup;
			APlayerCharacter::Control_Weapon2();
		}
		break;

	}
}
// Drops an item, only really called when the player picks up a weapon and is already holding one
void APlayerCharacter::Inv_DropItem(EEnumItemType ItemDropped)
{
	// Sets location, rotation, params of object
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();
	FActorSpawnParameters SpawnInfo;
	// Forces it to spawn regardless of collisions
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	// Spawns object
	AInteractable_Pickup_Item* SpawnedObject = GetWorld()->SpawnActorDeferred<AInteractable_Pickup_Item>(AInteractable_Pickup_Item::StaticClass(), FTransform(Rotation, Location, FVector(1.f, 1.f, 1.f)), this);
	// Sets object's type
	SpawnedObject->ItemType = ItemDropped;
	// Switch case to give it a mesh
	switch (ItemDropped)
	{
	case EEnumItemType::ITEM_WEAP_Pistol:
		SpawnedObject->ItemMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static")));
		break;
	case EEnumItemType::ITEM_WEAP_RifleExplosive:
	case EEnumItemType::ITEM_WEAP_RifleShock:
	case EEnumItemType::ITEM_WEAP_RifleStandard:
		SpawnedObject->ItemMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/M4A1_Static.M4A1_Static")));
		break;
	case EEnumItemType::ITEM_WEAP_ShotgunExplosive:
	case EEnumItemType::ITEM_WEAP_ShotgunShock:
	case EEnumItemType::ITEM_WEAP_ShotgunStandard:
		SpawnedObject->ItemMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/Mossberg_Static.Mossberg_Static")));
		break;
	default:
		SpawnedObject->ItemMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static")));
		break;
	}
	// Sets number within the object to 1
	SpawnedObject->ItemAmount = 1;
	UGameplayStatics::FinishSpawningActor(SpawnedObject, FTransform(Rotation, Location, FVector(1.f, 1.f, 1.f)));
}

float APlayerCharacter::Health_Get()
{
	// Returns player's health
	return PVar_PlayerHealth;
}
void APlayerCharacter::Health_Adjust(float changeValue)
{
	// Adjusts health by given value, minus values will increase
	// If the value is positive, then it is damaging...
	// Must not apply this *if* player has shield active
	if ((changeValue > 0)&&(!Pvar_IsShielded))
	{
		PVar_PlayerHealth -= changeValue;

		// Checks if health is 0 or below (aka dead)
		if (PVar_PlayerHealth <= 0)
		{
			// If dead, casts to game-mode and triggers a game-over scene
			if (ATrip_GameModeBase* CurGameMode = Cast<ATrip_GameModeBase>(GetWorld()->GetAuthGameMode()))
			{
				CurGameMode->GameOverScreen();
				UGameplayStatics::OpenLevel(GetWorld(), "Labs");
			}
		}
	} // If the value is negative, it is healing, always apply
	else if (changeValue < 0)
	{
		PVar_PlayerHealth -= changeValue;
		// If health goes above max, set back to max
		if (PVar_PlayerHealth > 100)
		{
			PVar_PlayerHealth = 100;
		}
	}
}

// Activates shielding, then starts timer that deactivates it
void APlayerCharacter::Activate_Shielding()
{
	// Doesn't allow multiple shields to be used at once
	// Only activates if the player has a shield in their inventory
	if ((!Pvar_IsShielded)&&(PVar_Shield > 0))
	{
		Pvar_IsShielded = true; // Activates shields
		PVar_Shield--; // Remove one shield
		// Starts 30 second timer that prevents multiple activations
		GetWorld()->GetTimerManager().SetTimer(TimerShieldPeriod, this, &APlayerCharacter::Deactivate_Shielding, 30.0f, false);
	}
}
void APlayerCharacter::Deactivate_Shielding()
{
	Pvar_IsShielded = false;
}