#pragma once

#include "CoreMinimal.h"
#include "Data/Inventory/HHM_Inventory.h"

#include "Data/Item/ItemData/HHM_ItemData.h"

#include "HHM_Inventory_List.generated.h"



USTRUCT()
struct FHHM_Inventory_List : public FHHM_Inventory
{
	GENERATED_USTRUCT_BODY()

public:
	FHHM_Inventory_List();

private:
	UPROPERTY()		//아이템 데이터를 ID와 함께 map에 저장함.
		TMap<int32, UHHM_ItemData*>		m_Container_ItemData;
	UPROPERTY()		//아이템의 ID를 배열에 저장하여 리스트 순서가 꼬이지 않게 함.
		TArray<int32>					m_Container_ItemDataIndex;

	UPROPERTY()
		int32							m_InventorySize = 0;

	

public:
	void	Set_InventorySize(int32 _size);

public:
	bool								IsInventoryFull() const;
	int32								Get_ItemNum() const;
	UHHM_ItemData*						Get_Item(int32 _index);
	const TMap<int32, UHHM_ItemData*>*	Get_ItemContainer_Const() const;
	TArray<UHHM_ItemData*>				Get_ItemList_Const() const;

public:
	EHHM_InventoryReturn	Item_Insert(int32& _index_Return, UHHM_ItemData* _pItemData);
	EHHM_InventoryReturn	Item_Pop_At(UHHM_ItemData*& _pItemData_Return, int32 _index);
	EHHM_InventoryReturn	Item_Remove(UHHM_ItemData* _pItemData);

	void					Clear_Inventory();



private:
	int32					Get_AvailiableItemID();
};