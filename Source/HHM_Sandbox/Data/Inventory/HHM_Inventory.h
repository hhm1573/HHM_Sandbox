#pragma once

#include "CoreMinimal.h"

#include "Base/Item/HHM_Data_Inventory_Item.h"
#include "Base/InventorySlot/HHM_Data_Inventory_Slot_Row.h"

#include "ObjectMacros.h"



USTRUCT()
struct FHHM_Inventory
{
	GENERATED_USTRUCT_BODY()

public:
	FHHM_Inventory();

protected:
	UPROPERTY()
		uint32		m_Size_Horizontal = 0;
	UPROPERTY()



};



FHHM_Inventory::FHHM_Inventory() {

}