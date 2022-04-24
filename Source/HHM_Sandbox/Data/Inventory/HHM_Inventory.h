#pragma once

#include "CoreMinimal.h"

#include "Base/HHM_Data_Inventory.h"
#include "Base/Item/HHM_Data_Inventory_Item.h"
#include "Base/InventorySlot/HHM_Data_Inventory_Slot_Row.h"
#include "Base/Enum_InventoryReturn.h"

#include "HHM_Inventory.generated.h"



USTRUCT(Blueprintable)
struct FHHM_Inventory
{
	GENERATED_USTRUCT_BODY()

public:
	FHHM_Inventory();

protected:
	UPROPERTY(BlueprintReadOnly)	//Collective of datas about inventory.
		FHHM_Data_Inventory								m_Data_Inventory;



public:
	virtual bool	Initialize_Inventory(const FHHM_Data_Inventory& _data_Inventory);

//public:
//	UFUNCTION(BlueprintCallable)
//		TMap<int32, FHHM_Data_Inventory_Item>& BP_Get_Container_ItemData();
};