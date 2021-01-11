#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"

UENUM(BlueprintType)
enum class EHHM_ItemType : uint8
{
	Item_Material		UMETA(DisplayName = "Item_Material"),
	Item_Placeable		UMETA(DisplayName = "Item_Placeable"),
	Item_Consumable		UMETA(DisplayName = "Item_Consumable"),
	Item_Weapon			UMETA(DisplayName = "Item_Weapon"),
	Item_Equipment		UMETA(DisplayName = "Item_Equipment"),
	Item_Unavailiable	UMETA(DisplayName = "Item_Unavailiable")
};