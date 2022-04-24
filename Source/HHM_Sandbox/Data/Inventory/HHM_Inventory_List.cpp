#include "Data/Inventory/HHM_Inventory_List.h"



FHHM_Inventory_List::FHHM_Inventory_List() {
	Clear_Inventory();
}



void FHHM_Inventory_List::Set_InventorySize(int32 _size)
{
	if (_size <= 0) {
		//Exception
		return;
	}

	m_InventorySize = _size;
}

bool FHHM_Inventory_List::IsInventoryFull() const
{
	int32 Num_Item = m_Container_ItemData.Num();
	return Num_Item >= m_InventorySize ? false : true;
}

int32 FHHM_Inventory_List::Get_ItemNum() const
{
	return m_Container_ItemData.Num();
}

UHHM_ItemData* FHHM_Inventory_List::Get_Item(int32 _index)
{
	int32 Index_Num = m_Container_ItemDataIndex.Num();
	if (_index >= Index_Num || _index < 0) {
		//Invalid Index
		return nullptr;
	}
	
	int32 ItemID = m_Container_ItemDataIndex[_index];
	bool IsValidID = m_Container_ItemData.Contains(ItemID);
	if (IsValidID == false) {
		//Exception Index has invalid item id
		return nullptr;
	}

	UHHM_ItemData* pItemData_Found = m_Container_ItemData[ItemID];
	
	return pItemData_Found;
}

const TMap<int32, UHHM_ItemData*>* FHHM_Inventory_List::Get_ItemContainer_Const() const
{
	return &m_Container_ItemData;
}

TArray<UHHM_ItemData*> FHHM_Inventory_List::Get_ItemList_Const() const
{
	//List Data Sync Check
	int32 Num_ItemData = m_Container_ItemData.Num();
	int32 Num_ItemIDIndex = m_Container_ItemDataIndex.Num();
	if (Num_ItemData != Num_ItemIDIndex) {
		//Exception 
	}

	//Build ItemList
	TArray<UHHM_ItemData*> ItemList;
	ItemList.Empty();

	for (int32 index = 0; index < Num_ItemIDIndex; ++index) {
		bool IsIDValid = m_Container_ItemData.Contains(m_Container_ItemDataIndex[index]);
		if (IsIDValid == false) {
			//Exception
			return TArray<UHHM_ItemData*>();
		}
		ItemList.Add(m_Container_ItemData[m_Container_ItemDataIndex[index]]);
	}

	return ItemList;
}



EHHM_InventoryReturn FHHM_Inventory_List::Item_Insert(int32& _index_Return, UHHM_ItemData* _pItemData)
{
	if (_pItemData == nullptr) {
		//Exception
		return EHHM_InventoryReturn::Return_NoItem;
	}

	int32 Num_Item = m_Container_ItemData.Num();
	if (Num_Item >= m_InventorySize) {
		return EHHM_InventoryReturn::Return_NoRoom;
	}

	int32 AvailiableID = Get_AvailiableItemID();
	UHHM_ItemData** ppItemData_Found = m_Container_ItemData.Find(AvailiableID);
	if (ppItemData_Found != nullptr) {
		//Exception Error Occured during finding availiable item id
		return EHHM_InventoryReturn::Return_Error;
	}
	if (AvailiableID < 0) {
		//Exception No Valid ID Found
		return EHHM_InventoryReturn::Return_NoRoom;
	}

	

	m_Container_ItemData.Add(AvailiableID, _pItemData);
	int32 Index_Added = m_Container_ItemDataIndex.Add(AvailiableID);
	
	_index_Return = Index_Added;

	return EHHM_InventoryReturn::Return_Succeed;
}

EHHM_InventoryReturn FHHM_Inventory_List::Item_Pop_At(UHHM_ItemData*& _pItemData_Return, int32 _index)
{
	bool IsValidIndex = m_Container_ItemDataIndex.Contains(_index);
	if (IsValidIndex == false) {
		//Exception
		return EHHM_InventoryReturn::Return_NoItem;
	}

	int32 InventoryItemID = m_Container_ItemDataIndex[_index];
	bool IsValidInventoryItemID = m_Container_ItemData.Contains(InventoryItemID);
	if (IsValidInventoryItemID == false) {
		//Exception
		m_Container_ItemDataIndex.RemoveAt(_index);
		return EHHM_InventoryReturn::Return_Error;
	}

	UHHM_ItemData* pItemData_Popped = m_Container_ItemData[InventoryItemID];
	if (pItemData_Popped == nullptr) {
		//Exception
		m_Container_ItemData.Remove(InventoryItemID);
		m_Container_ItemDataIndex.RemoveAt(_index);
		return EHHM_InventoryReturn::Return_Error;
	}

	

	_pItemData_Return = pItemData_Popped;
	m_Container_ItemData.Remove(InventoryItemID);
	m_Container_ItemDataIndex.RemoveAt(_index);
	return EHHM_InventoryReturn::Return_Succeed;
}

EHHM_InventoryReturn FHHM_Inventory_List::Item_Remove(UHHM_ItemData* _pItemData)
{
	if (_pItemData == nullptr) {
		//Exception
		return EHHM_InventoryReturn::Return_NoItem;
	}

	const int32* pKey = m_Container_ItemData.FindKey(_pItemData);
	if (pKey == nullptr) {
		//No Item Found
		return EHHM_InventoryReturn::Return_NoItem;
	}

	int32 Key = *pKey;
	m_Container_ItemData.Remove(Key);

	int32 ItemIndex = m_Container_ItemDataIndex.Find(Key);
	if (ItemIndex == INDEX_NONE) {
		return EHHM_InventoryReturn::Return_Error;
	}

	m_Container_ItemDataIndex.RemoveAt(ItemIndex);
	return EHHM_InventoryReturn::Return_Succeed;
}

void FHHM_Inventory_List::Clear_Inventory()
{
	m_Container_ItemData.Empty();
	m_Container_ItemDataIndex.Empty();
}

int32 FHHM_Inventory_List::Get_AvailiableItemID()
{
	const int32 InventoryHardLimit = 10000;
	int32 ID_Found = -1;

	for (int index = 0; index < InventoryHardLimit; ++index) {
		bool IsOccupied = m_Container_ItemData.Contains(index);
		if (IsOccupied == false) {
			ID_Found = index;
			break;
		}
	}

	return ID_Found;
}
