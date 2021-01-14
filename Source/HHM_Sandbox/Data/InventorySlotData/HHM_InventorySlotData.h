#pragma once

#include "CoreMinimal.h"
#include "HHM_InventorySlotData.generated.h"

USTRUCT()
struct FHHM_InventorySlotData
{
	GENERATED_USTRUCT_BODY()

public:
	bool	IsOccupied = false;
	int32	ItemID_Occupied = -1;

public:
	//Reset Current SlotData and set it as free
	void	Reset() {
		IsOccupied = false;
		ItemID_Occupied = -1;
	}

	void	Set_Occupied(int32 _itemID) {
		IsOccupied = true;
		ItemID_Occupied = _itemID;
	}
};