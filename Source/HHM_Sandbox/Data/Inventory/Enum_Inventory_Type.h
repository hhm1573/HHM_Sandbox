#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"

UENUM(BlueprintType)
enum class EHHM_Inventory_Type : uint8
{
	Inventory_Grid		UMETA(DisplayName = "Inventory_Grid"),
	Inventory_List		UMETA(DisplayName = "Inventory_List")
};