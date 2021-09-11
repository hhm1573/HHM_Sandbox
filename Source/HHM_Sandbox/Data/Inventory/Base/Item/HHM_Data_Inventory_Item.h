#pragma once

#include "CoreMinimal.h"

#include "Data/Item/ItemData/HHM_ItemData.h"

#include "HHM_Data_Inventory_Item.generated.h"



//Inventory ���� ItemData �� �����ϱ� ���� ������ ����ü
//ItemData�� �ش� �������� �����ϴ� Inventory Slot�� �����͸� ������.

USTRUCT(BlueprintType)
struct FHHM_Data_Inventory_Item
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HHM_Inventory)
		UHHM_ItemData* pItemData = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HHM_Inventory)
		TArray<int32>				Container_Index_Occupied;

public:
	FHHM_Data_Inventory_Item() {
		pItemData = nullptr;
		Container_Index_Occupied.Empty();
	}

	FHHM_Data_Inventory_Item(UHHM_ItemData* _pItem) {
		pItemData = _pItem;
		Container_Index_Occupied.Empty();
	}

	//public:
	//	UFUNCTION(BlueprintCallable)
	//		TArray<int32>& Get_Container_Index_Occupied() { return Container_Index_Occupied; }
};