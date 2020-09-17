// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryObject.h"
#include "Engine/UserDefinedStruct.h"
#include <Engine/Texture2D.h>

// Constructor, since no input is given, it default to a blank item slot
FInventoryObject::FInventoryObject()
{
	Inv_ItemType = EEnumItemType::ITEM_EMPTY;
	Inv_DisplayName = "Empty";
	Inv_ItemAmount = 0;
	Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Default.Inventory_Default"));
	Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static"));
}

// Constructor with Switch, allows any item to be generated
FInventoryObject::FInventoryObject(EEnumItemType InventoryPick)
{
	switch (InventoryPick)
	{
	case EEnumItemType::ITEM_AMMO_Pistol:
		Inv_ItemType = EEnumItemType::ITEM_AMMO_Pistol;
		Inv_DisplayName = "Pistol Ammo";
		Inv_ItemAmount = 0;
		Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Default.Inventory_Default"));
		Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static"));
		break;

	case EEnumItemType::ITEM_AMMO_Rifle:
		Inv_ItemType = EEnumItemType::ITEM_AMMO_Rifle;
		Inv_DisplayName = "Rifle Ammo";
		Inv_ItemAmount = 0;
		Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Default.Inventory_Default"));
		Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static"));
		break;

	case EEnumItemType::ITEM_AMMO_Shotgun:
		Inv_ItemType = EEnumItemType::ITEM_AMMO_Shotgun;
		Inv_DisplayName = "Shotgun Shells";
		Inv_ItemAmount = 0;
		Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Default.Inventory_Default"));
		Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static"));
		break;

	case EEnumItemType::ITEM_PICK_Camo:
		Inv_ItemType = EEnumItemType::ITEM_PICK_Camo;
		Inv_DisplayName = "Camouflage Module";
		Inv_ItemAmount = 0;
		Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Default.Inventory_Default"));
		Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static"));
		break;

	case EEnumItemType::ITEM_PICK_Healthpack:
		Inv_ItemType = EEnumItemType::ITEM_PICK_Healthpack;
		Inv_DisplayName = "First Aid Kit";
		Inv_ItemAmount = 0;
		Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Default.Inventory_Default"));
		Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static"));
		break;

	case EEnumItemType::ITEM_PICK_Keycard:
		Inv_ItemType = EEnumItemType::ITEM_PICK_Keycard;
		Inv_DisplayName = "Keycard";
		Inv_ItemAmount = 0;
		Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Default.Inventory_Default"));
		Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static"));
		break;

	case EEnumItemType::ITEM_PICK_ScrapMetal:
		Inv_ItemType = EEnumItemType::ITEM_PICK_ScrapMetal;
		Inv_DisplayName = "Recyclable Metal";
		Inv_ItemAmount = 0;
		Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Default.Inventory_Default"));
		Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static"));
		break;

	case EEnumItemType::ITEM_PICK_Shielding:
		Inv_ItemType = EEnumItemType::ITEM_PICK_Shielding;
		Inv_DisplayName = "Shield Module";
		Inv_ItemAmount = 0;
		Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Default.Inventory_Default"));
		Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static"));
		break;

	case EEnumItemType::ITEM_WEAP_GrenadeExplosive:
		Inv_ItemType = EEnumItemType::ITEM_WEAP_GrenadeExplosive;
		Inv_DisplayName = "Frag Grenade";
		Inv_ItemAmount = 0;
		Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Default.Inventory_Default"));
		Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static"));
		break;

	case EEnumItemType::ITEM_WEAP_GrenadeShock:
		Inv_ItemType = EEnumItemType::ITEM_WEAP_GrenadeShock;
		Inv_DisplayName = "EMP Grenade";
		Inv_ItemAmount = 0;
		Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Default.Inventory_Default"));
		Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static"));
		break;

	case EEnumItemType::ITEM_WEAP_GrenadeSticky:
		Inv_ItemType = EEnumItemType::ITEM_WEAP_GrenadeSticky;
		Inv_DisplayName = "Adhesive Grenade";
		Inv_ItemAmount = 0;
		Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Default.Inventory_Default"));
		Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static"));
		break;

	case EEnumItemType::ITEM_WEAP_Pistol:
		Inv_ItemType = EEnumItemType::ITEM_WEAP_Pistol;
		Inv_DisplayName = "Pistol";
		Inv_ItemAmount = 0;
		Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Pistol.Inventory_Pistol"));
		Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static"));
		break;

	case EEnumItemType::ITEM_WEAP_RifleExplosive:
		Inv_ItemType = EEnumItemType::ITEM_WEAP_RifleExplosive;
		Inv_DisplayName = "Antimatter Rifle";
		Inv_ItemAmount = 0;
		Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Rifle.Inventory_Rifle"));
		Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/M4A1_Static.M4A1_Static"));
		break;

	case EEnumItemType::ITEM_WEAP_RifleShock:
		Inv_ItemType = EEnumItemType::ITEM_WEAP_RifleShock;
		Inv_DisplayName = "Laser Rifle";
		Inv_ItemAmount = 0;
		Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Rifle.Inventory_Rifle"));
		Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/WeaponsM4A1_Static.M4A1_Static"));
		break;

	case EEnumItemType::ITEM_WEAP_RifleStandard:
		Inv_ItemType = EEnumItemType::ITEM_WEAP_RifleStandard;
		Inv_DisplayName = "Marksman Rifle";
		Inv_ItemAmount = 0;
		Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Rifle.Inventory_Rifle"));
		Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/M4A1_Static.M4A1_Static"));
		break;

	case EEnumItemType::ITEM_WEAP_ShotgunExplosive:
		Inv_ItemType = EEnumItemType::ITEM_WEAP_ShotgunExplosive;
		Inv_DisplayName = "The Doomsday Gun";
		Inv_ItemAmount = 0;
		Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Shotgun.Inventory_Shotgun"));
		Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/Mossberg_Static.Mossberg_Static"));
		break;

	case EEnumItemType::ITEM_WEAP_ShotgunShock:
		Inv_ItemType = EEnumItemType::ITEM_WEAP_ShotgunShock;
		Inv_DisplayName = "Stun Shotgun";
		Inv_ItemAmount = 0;
		Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Shotgun.Inventory_Shotgun"));
		Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/Mossberg_Static.Mossberg_Static"));
		break;

	case EEnumItemType::ITEM_WEAP_ShotgunStandard:
		Inv_ItemType = EEnumItemType::ITEM_WEAP_ShotgunStandard;
		Inv_DisplayName = "Combat Shotgun";
		Inv_ItemAmount = 0;
		Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Shotgun.Inventory_Shotgun"));
		Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/Mossberg_Static.Mossberg_Static"));
		break;

	case EEnumItemType::ITEM_EMPTY:
		Inv_ItemType = EEnumItemType::ITEM_EMPTY;
		Inv_DisplayName = "Empty";
		Inv_ItemAmount = 0;
		Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Default.Inventory_Default"));
		Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static"));
		break;

	default:
		Inv_ItemType = EEnumItemType::ITEM_EMPTY;
		Inv_DisplayName = "Empty";
		Inv_ItemAmount = 0;
		Inv_ItemIcon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Images/Inventory_Default.Inventory_Default"));
		Inv_ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Weapons/USP_Static.USP_Static"));
		break;
	}
}