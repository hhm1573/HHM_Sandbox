#pragma once

#include "CoreMinimal.h"

#include "Data/Item/ItemData/HHM_ItemData.h"

#include "HHM_Data_Inventory_Item.generated.h"



//Inventory 내에 ItemData 를 저장하기 위한 데이터 구조체
//ItemData와 해당 아이템이 점유하는 Inventory Slot의 데이터를 가진다.

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