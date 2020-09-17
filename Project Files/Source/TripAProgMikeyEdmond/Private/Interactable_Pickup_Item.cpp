// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable_Pickup_Item.h"
#include "EnumItemType.h"
#include "PlayerCharacter.h"
#include "Components/StaticMeshComponent.h"

AInteractable_Pickup_Item::AInteractable_Pickup_Item()
{
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);
	ItemMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static")));
	ItemMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ItemMesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AInteractable_Pickup_Item::BeginPlay()
{
	// Switch case sets the mesh for the item
	Super::BeginPlay();
	switch (ItemType)
	{
		// Pistol
	case EEnumItemType::ITEM_WEAP_Pistol:
		ItemMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static")));
		break;
		// Rifles
	case EEnumItemType::ITEM_WEAP_RifleStandard:
	case EEnumItemType::ITEM_WEAP_RifleExplosive:
	case EEnumItemType::ITEM_WEAP_RifleShock:
		ItemMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/M4A1_Static.M4A1_Static")));
		break;
		// Shotguns
	case EEnumItemType::ITEM_WEAP_ShotgunStandard:
	case EEnumItemType::ITEM_WEAP_ShotgunExplosive:
	case EEnumItemType::ITEM_WEAP_ShotgunShock:
		ItemMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/Mossberg_Static.Mossberg_Static")));
		break;
		// Grenades
	case EEnumItemType::ITEM_WEAP_GrenadeExplosive:
	case EEnumItemType::ITEM_WEAP_GrenadeShock:
	case EEnumItemType::ITEM_WEAP_GrenadeSticky:
		ItemMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static")));
		break;
		// Ammunition
	case EEnumItemType::ITEM_AMMO_Pistol:
		ItemMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static")));
		break;
	case EEnumItemType::ITEM_AMMO_Rifle:
		ItemMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static")));
		break;
	case EEnumItemType::ITEM_AMMO_Shotgun:
		ItemMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static")));
		break;
		// Pickups
	case EEnumItemType::ITEM_PICK_Camo:
		ItemMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static")));
		break;
	case EEnumItemType::ITEM_PICK_Healthpack:
		ItemMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static")));
		break;
	case EEnumItemType::ITEM_PICK_Keycard:
		ItemMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Key/msh_Key.msh_Key")));
		break;
	case EEnumItemType::ITEM_PICK_Shielding:
		ItemMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static")));
		break;
	case EEnumItemType::ITEM_PICK_ScrapMetal:
		ItemMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static")));
		break;

	}
}

// Contains functionality for the object
void AInteractable_Pickup_Item::TrigInteraction(APlayerCharacter* PlayerChar)
{
	PlayerChar->Inv_PickupItem(ItemType, ItemAmount);
	Destroy();
}