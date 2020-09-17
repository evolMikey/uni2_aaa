// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InventoryObject.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Projectile_Player.h"
#include "AIChar_Abstract_Turret.h"
#include "ShootRangeTarget.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class TRIPAPROGMIKEYEDMOND_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	// Components for the character
	class UCameraComponent* PlayerCamera;
	class USphereComponent* Interact_Radius;

	// Static Mesh used to represent the player's gun
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* PlayerHeldGun;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> wPlayerHUD;
	UUserWidget* PlayerHUDWidget;

	// Variables stored for later use by the player
protected:
	float PVar_Speed = 3; // Speed player moves at, halved on crouching
	float PVar_JumpHeight = 500; // Height player can jump to
	float PVar_WeaponDamage = 25; // Amount of damage the held weapon can do when fired
	float PVar_WeaponRange = 1500; // Range of the weapon
	float PVar_MeleeDamage = 50; // Amount of damage the held weapon can do when swung like a club
	bool PVar_Crouching = false; // Boolean for player crouching
	float PVar_TriggerRadius = 100; // Radius that player can interact at
	float PVar_PlayerHealth = 100;
	bool Pvar_IsShielded = false; // If a shield is equip, prevent damage
	int PVar_Weap1Ammo = 9;
	int PVar_Weap2Ammo = 0;

	// Variables relating to interactable objects
	bool PVar_IsSeekingInteract; // If an object requires looking at, this is true, causes linetrace to be called on every tick
	UPROPERTY()
		class AAbstract_Interactable* PVar_InteractObject; // Stores reference to the object that can be interacted with

public:
	// Player holds 2 guns at once, only one is equip though (determined by boolean below)
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		EEnumItemType PVar_Weapon1 = EEnumItemType::ITEM_WEAP_Pistol;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	EEnumItemType PVar_Weapon2 = EEnumItemType::ITEM_EMPTY;
	bool PVar_EquipGun1 = true;
	// Various pickups and collectables
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		uint8 PVar_Healthpack = 0;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		uint8 PVar_Camo = 0;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		uint8 PVar_Shield = 0;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		uint8 PVar_Keycard = 0;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		uint8 PVar_ScrapMetal = 0;
	// Ammo counters, player always carries ammo even if they don't have the gun
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		uint8 PVar_RifleAmmo = 0;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		uint8 PVar_ShotgunAmmo = 0;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		uint8 PVar_PistolAmmo = 9;
	// Grenade counters, same as ammo but can be equip without a "gun" because they're standalone
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		uint8 PVar_GrenadeEmp = 0;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		uint8 PVar_GrenadeStick = 0;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		uint8 PVar_GrenadeFrag = 0;
	// 0 = melee, 1 = frag, 2 = emp, 3 = stick
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		uint8 PVar_EquipGrenade = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool PVar_HasFoundObject = false; // Used by UI to detect if object is being looked at

	// Subclass for projectile
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AProjectile_Player> ProjectileClass;


public:
	// Sets default values for this character's properties
	APlayerCharacter();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Functions for player inputs
protected:
	// Functions for movement/looking
	void Control_Movement_Forward(float AxisInput);
	void Control_Movement_Side(float AxisInput);
	void Control_Look_Horizontal(float AxisInput);
	void Control_Look_Vertical(float AxisInput);
	void Control_Crouch();
	void Control_Jump();
	// Functions for fighting
	void Control_FireWeapon();
	void Control_ReloadWeapon();
	FTimerHandle TimerReload;
	bool bIsReloading = false;
	void ReloadWaitTime();
	void Control_AimDownSights();
	void Control_MeleeAttack();
	// Functions for misc controls
	void Control_Item1();
	void Control_Item2();
	void Control_Item3();
	void Control_Item4();
	void Control_Weapon1();
	void Control_Weapon2();
	void Control_Interact();
	void Control_Pause();

	//Function for linetracing
	FHitResult LineTraceFunc(float TraceDistance, ECollisionChannel TChannel, bool DebugPieces);

public:
	// Functions relating to overlapping
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintCallable)
		float Health_Get();
	UFUNCTION(BlueprintCallable)
		void Health_Adjust(float changeValue);
	UFUNCTION()
		void Activate_Shielding();
	UFUNCTION()
		void Deactivate_Shielding();
		FTimerHandle TimerShieldPeriod;

	// Functions relating to picking up/dropping/switching objects around
public:
	void Inv_PickupItem(EEnumItemType ItemPickup, int ItemAmount);
protected:
	void Inv_DropItem(EEnumItemType ItemDropped);

};