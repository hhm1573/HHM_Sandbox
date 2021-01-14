#pragma once

#include "CoreMinimal.h"

#include "Data/InventorySlotData/HHM_InventorySlotData.h"

#include "HHM_InventorySlotData_Row.generated.h"

USTRUCT()
struct FHHM_InventorySlotData_Row
{
	GENERATED_USTRUCT_BODY()

public:
	TArray<FHHM_InventorySlotData>		SlotDataRow;
};