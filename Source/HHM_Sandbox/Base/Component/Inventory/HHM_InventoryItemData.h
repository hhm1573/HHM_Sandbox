#pragma once

#include "CoreMinimal.h"

#include "Data/Item/ItemData/HHM_ItemData.h"

#include "HHM_InventoryItemData.generated.h"


USTRUCT(BlueprintType)
struct FHHM_InventoryItemData
{
	GENERATED_USTRUCT_BODY();

public:
	TSharedPtr<UHHM_ItemData>	pItem = nullptr;
	UHHM_ItemData*				RawPointer_Item = nullptr;
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

//public:
//	UFUNCTION(BlueprintCallable)
//		TArray<int32>& Get_Container_Index_Occupied() { return Container_Index_Occupied; }
};