#pragma once

#include "CoreMinimal.h"
#include "HHM_Data_Inventory_Slot.generated.h"

USTRUCT()
struct FHHM_Data_Inventory_Slot
{
	GENERATED_USTRUCT_BODY()

public:
	bool	IsOccupied = false;
	bool	IsSource = false;		//Does item starts here?
	int32	ItemID_Occupied = -1;

public:
	//Reset Current SlotData and set it as free
	void	Reset() {
		IsOccupied = false;
		IsSource = false;
		ItemID_Occupied = -1;
	}

	void	Set_Occupied(int32 _itemID) {
		IsOccupied = true;
		ItemID_Occupied = _itemID;
	}

	void	Set_Source(bool _isSource) {
		IsSource = _isSource;
	}
};