#pragma once

#include "CoreMinimal.h"

#include "Data/Item/ItemData/HHM_ItemData.h"

#include "HHM_InventoryItemData.generated.h"


USTRUCT()
struct FHHM_InventoryItemData
{
	GENERATED_USTRUCT_BODY();

public:
	TSharedPtr<UHHM_ItemData>	pItem = nullptr;
	TArray<int32>				Container_Index_Occupied;

public:
	FHHM_InventoryItemData() {
		pItem = nullptr;
		Container_Index_Occupied.Empty();
	}

	FHHM_InventoryItemData(TSharedPtr<UHHM_ItemData>& _pItem) {
		pItem = _pItem;
		Container_Index_Occupied.Empty();
	}
};