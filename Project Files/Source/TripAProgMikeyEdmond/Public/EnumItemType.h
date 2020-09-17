// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EnumItemType.generated.h"

UENUM(BlueprintType)
enum class EEnumItemType : uint8
{
	ITEM_WEAP_Pistol,
	ITEM_WEAP_RifleStandard,
	ITEM_WEAP_RifleShock,
	ITEM_WEAP_RifleExplosive,
	ITEM_WEAP_ShotgunStandard,
	ITEM_WEAP_ShotgunShock,
	ITEM_WEAP_ShotgunExplosive,
	ITEM_WEAP_GrenadeShock,
	ITEM_WEAP_GrenadeExplosive,
	ITEM_WEAP_GrenadeSticky,
	ITEM_AMMO_Pistol,
	ITEM_AMMO_Rifle,
	ITEM_AMMO_Shotgun,
	ITEM_PICK_ScrapMetal,
	ITEM_PICK_Keycard,
	ITEM_PICK_Healthpack,
	ITEM_PICK_Camo,
	ITEM_PICK_Shielding,
	ITEM_EMPTY
};

template<typename T>
static FString EnumToString(const FString& enumName, const T value)
{
	UEnum* pEnum = FindObject<UEnum>(ANY_PACKAGE, *enumName);
	return *(pEnum ? pEnum->GetNameStringByIndex(static_cast<uint8>(value)) : "null");
}