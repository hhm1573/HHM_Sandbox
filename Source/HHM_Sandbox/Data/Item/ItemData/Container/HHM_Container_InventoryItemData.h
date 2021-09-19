#pragma once

#include "EngineMinimal.h"

#include "Data/Inventory/Base/Item/HHM_Data_Inventory_Item.h"

#include "HHM_Container_InventoryItemData.generated.h"

USTRUCT(BlueprintType)
struct FHHM_Container_InventoryItemData
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HHM_Inventory)
	TMap<int32, FHHM_Data_Inventory_Item> Container_InventoryItemData;

public:
	FHHM_Container_InventoryItemData() {
		Container_InventoryItemData.Empty();
	}
};