#include "Data/Inventory/HHM_Inventory_List.h"



UHHM_Inventory_List::UHHM_Inventory_List() {
	Clear_Inventory();
}



bool UHHM_Inventory_List::BP_Initialize_Inventory(FString _name_Inventory, float _time_Interact, int32 _size_Inventory)
{
	return Initialize_Inventory(_name_Inventory, _time_Interact, _size_Inventory);
}

bool UHHM_Inventory_List::BP_Set_InventorySize(int32 _size)
{
	return Set_InventorySize(_size);
}



int32 UHHM_Inventory_List::BP_Get_InventorySize()
{
	return Get_InventorySize();
}

TArray<UHHM_ItemData*> UHHM_Inventory_List::BP_Get_ItemList()
{
	return Get_ItemList_Const();
}

int32 UHHM_Inventory_List::BP_Get_NumItem()
{
	return m_Container_ItemData.Num();
}



EHHM_InventoryReturn UHHM_Inventory_List::BP_Item_Insert(int32& _index_Return, UHHM_ItemData* _pItemData)
{
	return Item_Insert(_index_Return, _pItemData);
}

EHHM_InventoryReturn UHHM_Inventory_List::BP_Item_Pop_At(UHHM_ItemData*& _pItemData_Return, int32 _index)
{
	return Item_Pop_At(_pItemData_Return, _index);
}

EHHM_InventoryReturn UHHM_Inventory_List::BP_Item_Remove(UHHM_ItemData* _pItemData)
{
	return Item_Remove(_pItemData);
}

void UHHM_Inventory_List::BP_Clear_Inventory()
{
	Clear_Inventory();
}



bool UHHM_Inventory_List::Initialize_Inventory(FString _name_Inventory, float _time_Interact, int32 _size_Inventory)
{
	bool IsNameEmpty = _name_Inventory.IsEmpty();
	if (IsNameEmpty == true) {
		//Exception
		return false;
	}

	if (_time_Interact <= 0.0f) {
		//Exception
		return false;
	}

	

	const bool IsSucceed_SetInventorySize = Set_InventorySize(_size_Inventory);
	if (IsSucceed_SetInventorySize == false) {
		//Exception
		return false;
	}



	m_Data_Inventory.Initialize(_name_Inventory, _time_Interact, 1, _size_Inventory);

	return true;
}

bool UHHM_Inventory_List::Set_InventorySize(int32 _size)
{
	if (_size <= 0) {
		//Exception
		return false;
	}

	//m_InventorySize = _size;
	m_Data_Inventory.m_Size_Vertical = _size;

	return true;
}

int32 UHHM_Inventory_List::Get_InventorySize() const
{
	return m_Data_Inventory.m_Size_Vertical;
}

bool UHHM_Inventory_List::IsInventoryFull() const
{
	int32 Num_Item = m_Container_ItemData.Num();
	return Num_Item >= m_Data_Inventory.m_Size_Vertical ? false : true;
}

int32 UHHM_Inventory_List::Get_ItemNum() const
{
	return m_Container_ItemData.Num();
}

UHHM_ItemData* UHHM_Inventory_List::Get_Item(int32 _index)
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

const TMap<int32, UHHM_ItemData*>* UHHM_Inventory_List::Get_ItemContainer_Const() const
{
	return &m_Container_ItemData;
}

TArray<UHHM_ItemData*> UHHM_Inventory_List::Get_ItemList_Const() const
{
	//Build ItemList
	TArray<UHHM_ItemData*> ItemList;
	ItemList.Empty();



	//List Data Sync Check
	int32 Num_ItemData = m_Container_ItemData.Num();
	int32 Num_ItemIDIndex = m_Container_ItemDataIndex.Num();
	if (Num_ItemData != Num_ItemIDIndex) {
		//Exception 
		return ItemList;
	}

	

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



EHHM_InventoryReturn UHHM_Inventory_List::Item_Insert(int32& _index_Return, UHHM_ItemData* _pItemData)
{
	if (_pItemData == nullptr) {
		//Exception
		return EHHM_InventoryReturn::Return_NoItem;
	}

	int32 Num_Item = m_Container_ItemData.Num();
	if (Num_Item >= m_Data_Inventory.m_Size_Vertical) {
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

	//Fire InventoryUpdate Event
	OnInventoryUpdate.Broadcast(this);

	return EHHM_InventoryReturn::Return_Succeed;
}

EHHM_InventoryReturn UHHM_Inventory_List::Item_Pop_At(UHHM_ItemData*& _pItemData_Return, int32 _index)
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

	//Fire InventoryUpdate Event
	OnInventoryUpdate.Broadcast(this);

	return EHHM_InventoryReturn::Return_Succeed;
}

EHHM_InventoryReturn UHHM_Inventory_List::Item_Remove(UHHM_ItemData* _pItemData)
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

	//Fire InventoryUpdate Event
	OnInventoryUpdate.Broadcast(this);

	return EHHM_InventoryReturn::Return_Succeed;
}

void UHHM_Inventory_List::Clear_Inventory()
{
	m_Container_ItemData.Empty();
	m_Container_ItemDataIndex.Empty();

	//Fire InventoryUpdate Event
	OnInventoryUpdate.Broadcast(this);
}

int32 UHHM_Inventory_List::Get_AvailiableItemID()
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
