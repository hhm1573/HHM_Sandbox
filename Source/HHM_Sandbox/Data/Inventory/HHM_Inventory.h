#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Base/HHM_Data_Inventory.h"
#include "Base/Item/HHM_Data_Inventory_Item.h"
#include "Base/InventorySlot/HHM_Data_Inventory_Slot_Row.h"
#include "Base/Enum_InventoryReturn.h"

#include "HHM_Inventory.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHHM_Delegate_InventoryUpdate, UHHM_Inventory*, Inventory);

UCLASS(Blueprintable)
class HHM_SANDBOX_API UHHM_Inventory : public UObject
{
	GENERATED_BODY()

public:
	UHHM_Inventory();

public:
	UPROPERTY(BlueprintAssignable, Category = "HHM_Event")
		FHHM_Delegate_InventoryUpdate OnInventoryUpdate;

protected:
	UPROPERTY()	//Collective of datas about inventory.
		FHHM_Data_Inventory								m_Data_Inventory = FHHM_Data_Inventory();



public:
	UFUNCTION(BlueprintCallable)
		FHHM_Data_Inventory	BP_Get_InventoryData();



//public:
//	virtual bool	Initialize_Inventory(const FHHM_Data_Inventory& _data_Inventory);

//public:
//	UFUNCTION(BlueprintCallable)
//		TMap<int32, FHHM_Data_Inventory_Item>& BP_Get_Container_ItemData();
};