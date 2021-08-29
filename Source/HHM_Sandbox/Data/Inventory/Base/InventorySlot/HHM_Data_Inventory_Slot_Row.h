#pragma once

#include "CoreMinimal.h"

#include "Data/Inventory/Base/InventorySlot/HHM_Data_Inventory_Slot.h"

#include "HHM_Data_Inventory_Slot_Row.generated.h"

USTRUCT()
struct FHHM_Data_Inventory_Slot_Row
{
	GENERATED_USTRUCT_BODY()

public:
	TArray<FHHM_Data_Inventory_Slot>		SlotDataRow;
};