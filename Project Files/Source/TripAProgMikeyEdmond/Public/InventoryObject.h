// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "EnumItemType.h"
#include <Engine/Texture2D.h>
#include "InventoryObject.generated.h"

USTRUCT()
struct FInventoryObject
{
	GENERATED_BODY()
	EEnumItemType Inv_ItemType; // Item type, based on enum
	FString Inv_DisplayName; // Display name, for inventories and stuff
	uint8 Inv_ItemAmount; // Amount of items in this stack currently
	UTexture2D* Inv_ItemIcon; // Texture used for the item in inventory
	UStaticMesh* Inv_ItemMesh; // Mesh used by this item, if any

public:
	FInventoryObject();
	FInventoryObject(EEnumItemType InventoryPick);
};