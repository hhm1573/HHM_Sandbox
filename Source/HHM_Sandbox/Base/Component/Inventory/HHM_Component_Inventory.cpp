// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Component_Inventory.h"

#include "Item/Base/HHM_Item.h"



constexpr int32 INVENTORY_HARD_LIMIT = 100;



// Sets default values for this component's properties
UHHM_Component_Inventory::UHHM_Component_Inventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHHM_Component_Inventory::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHHM_Component_Inventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



int32 UHHM_Component_Inventory::Inventory_Add(const bool& _isRoot, const FHHM_Data_Inventory& _data_Inventory)
{
	//Valid check
	if (_data_Inventory.m_Size_Horizontal <= 0 || _data_Inventory.m_Size_Vertical <= 0) {
		//Exception too small inventory size
		return -1;
	}

	

	//Get Available Inventory ID
	int32 InventoryID_Found = Get_AvailableInventoryID(_isRoot);
	if (InventoryID_Found < 0) {
		//Exception Getting Available inventory id failed
		return -1;
	}

	//Make inventory and initialize it
	TMap<int32, FHHM_Inventory>* pContainer_Inventory_ToAdd = nullptr;
	if (_isRoot) {
		pContainer_Inventory_ToAdd = &m_Container_Inventory_Root;
	}
	else {
		pContainer_Inventory_ToAdd = &m_Container_Inventory;
	}

	bool IsOccupied_ID = pContainer_Inventory_ToAdd->Contains(InventoryID_Found);
	if (IsOccupied_ID == true) {
		//Exception found available inventory id is not available. check the Get_AvailableInventoryID function
		return -1;
	}

	pContainer_Inventory_ToAdd->Add(InventoryID_Found, FHHM_Inventory());
	bool IsSucceed_Initialize = (*pContainer_Inventory_ToAdd)[InventoryID_Found].Initialize_Inventory(_data_Inventory);
	if (IsSucceed_Initialize == false) {
		//Exception Inventory Initialize failed
		pContainer_Inventory_ToAdd->Remove(InventoryID_Found);
		return -1;
	}



	return InventoryID_Found;
}



UHHM_ItemData* UHHM_Component_Inventory::Get_ItemDataPtr(const bool& _isRoot, const int32& _inventoryID, const int32& _index_Horizontal, const int32& _index_Vertical)
{
	bool IsValidIndex = Check_IsValidIndex(_isRoot, _inventoryID, _index_Horizontal, _index_Vertical);
	if (IsValidIndex == false) {
		//Exception Invalid Index
		return nullptr;
	}



	TMap<int32, FHHM_Inventory>* pContainer_Inventory_Target = nullptr;
	if (_isRoot) {
		pContainer_Inventory_Target = &m_Container_Inventory_Root;
	}
	else {
		pContainer_Inventory_Target = &m_Container_Inventory;
	}

	UHHM_ItemData* pItemData_Return = nullptr;
	EHHM_InventoryReturn InventoryReturn = (*pContainer_Inventory_Target)[_inventoryID].Get_ItemDataPtr_AtSlot(pItemData_Return, _index_Horizontal, _index_Vertical);
	if (InventoryReturn != EHHM_InventoryReturn::Return_Succeed) {
		//Exception Get ItemData failed
		return nullptr;
	}

	return pItemData_Return;
}

//TMap<int32, FHHM_Data_Inventory_Item>& UHHM_Component_Inventory::Get_ItemDataContainer_Ref(const bool& _isRoot, const int32& _inventoryID)
//{
//	TMap<int32, FHHM_Inventory>* pContainer_Inventory_Target = nullptr;
//	if (_isRoot) {
//		pContainer_Inventory_Target = &m_Container_Inventory_Root;
//	}
//	else {
//		pContainer_Inventory_Target = &m_Container_Inventory;
//	}
//
//	bool IsOccupied = pContainer_Inventory_Target->Contains(_inventoryID);
//	if (IsOccupied == false) {
//		//Exception Invalid inventory ID
//		예외상황시에 아무것도 리턴할수 없고, 대충 아무거나 생성해서 리턴하자니 bool값이 아니고 레퍼런스라 외부에서 함수의 성공여부를 알기 힘들다.
//		함수를 고칠까 하였지만 딱히 쓰일일도 거의 없을것 같고 귀찮아서 미뤄두었다.
//	}
//}

bool UHHM_Component_Inventory::Get_InventorySize(FIntPoint& _inventorySize_Return, const bool& _isRoot, const int32& _inventoryID)
{
	bool IsValid_InventoryID = Check_IsValidInventoryID(_isRoot, _inventoryID);
	if (IsValid_InventoryID == false) {
		//Exception Invalid Inventory ID
		return false;
	}

	TMap<int32, FHHM_Inventory>* pContainer_Inventory_Target = nullptr;
	if (_isRoot) {
		pContainer_Inventory_Target = &m_Container_Inventory_Root;
	}
	else {
		pContainer_Inventory_Target = &m_Container_Inventory;
	}

	FIntPoint InventorySize_Return = (*pContainer_Inventory_Target)[_inventoryID].Get_InventorySize();

	_inventorySize_Return = InventorySize_Return;

	return true;
}



bool UHHM_Component_Inventory::Check_IsValidIndex(const bool& _isRoot, const int32& _inventoryID, const int32& _index_Horizontal, const int32& _index_Vertical)
{
	bool IsValidInventoryID = Check_IsValidInventoryID(_isRoot, _inventoryID);
	if (IsValidInventoryID == false) {
		return false;
	}

	TMap<int32, FHHM_Inventory>* pContainer_Inventory_Target = nullptr;
	if (_isRoot) {
		pContainer_Inventory_Target = &m_Container_Inventory_Root;
	}
	else {
		pContainer_Inventory_Target = &m_Container_Inventory;
	}

	FIntPoint InventorySize = (*pContainer_Inventory_Target)[_inventoryID].Get_InventorySize();

	return _index_Horizontal < 0 || _index_Horizontal >= InventorySize.X
		|| _index_Vertical < 0 || _index_Vertical >= InventorySize.Y ? false : true;
}

bool UHHM_Component_Inventory::Check_IsValidInventoryID(const bool& _isRoot, const int32& _inventoryID) {
	TMap<int32, FHHM_Inventory>* pContainer_Inventory_Target = nullptr;
	if (_isRoot) {
		pContainer_Inventory_Target = &m_Container_Inventory_Root;
	}
	else {
		pContainer_Inventory_Target = &m_Container_Inventory;
	}

	bool IsValidInventoryID = pContainer_Inventory_Target->Contains(_inventoryID);
	return IsValidInventoryID;
}

bool UHHM_Component_Inventory::Check_IsItemSwappable(const bool& _isRoot, const int32& _inventoryID, const int32& _index_Horizontal, const int32& _index_Vertical, const UHHM_ItemData*& _pItemData_Swap)
{
	bool IsValidInventoryID = Check_IsValidInventoryID(_isRoot, _inventoryID);
	if (IsValidInventoryID == false) {
		//Exception invalid inventory id
		return false;
	}

	TMap<int32, FHHM_Inventory>* pContainer_Inventory_Target = nullptr;
	if (_isRoot) {
		pContainer_Inventory_Target = &m_Container_Inventory_Root;
	}
	else {
		pContainer_Inventory_Target = &m_Container_Inventory;
	}

	bool IsSwappable = (*pContainer_Inventory_Target)[_inventoryID].Check_IsItemSwappable(_index_Horizontal, _index_Vertical, _pItemData_Swap);
	return IsSwappable;
}

bool UHHM_Component_Inventory::Check_IsItemInsertable(const bool& _isRoot, const int32& _inventoryID, const UHHM_ItemData*& _pItemData)
{
	bool IsValidInventoryID = Check_IsValidInventoryID(_isRoot, _inventoryID);
	if (IsValidInventoryID == false) {
		//Exception Invalid inventory id
		return false;
	}

	TMap<int32, FHHM_Inventory>* pContainer_Inventory_Target = nullptr;
	if (_isRoot) {
		pContainer_Inventory_Target = &m_Container_Inventory_Root;
	}
	else {
		pContainer_Inventory_Target = &m_Container_Inventory;
	}

	bool IsInsertable = (*pContainer_Inventory_Target)[_inventoryID].Check_IsItemInsertable(_pItemData);
	return IsInsertable;
}

bool UHHM_Component_Inventory::Check_IsItemInsertableAt(const bool& _isRoot, const int32& _inventoryID, const int32& _index_Horizontal, const int32& _index_Vertical, const UHHM_ItemData*& _pItemData_Insert)
{
	//bool IsValidInventoryID = Check_IsValidInventoryID(_isRoot, _inventoryID);
	//if (IsValidInventoryID == false) {
	//	//Exception invalid inventory id
	//	return false;
	//}

	bool IsValidIndex = Check_IsValidIndex(_isRoot, _inventoryID, _index_Horizontal, _index_Vertical);
	if (IsValidIndex == false) {
		//Exception invalid index
		return false;
	}

	TMap<int32, FHHM_Inventory>* pContainer_Inventory_Target = nullptr;
	if (_isRoot) {
		pContainer_Inventory_Target = &m_Container_Inventory_Root;
	}
	else {
		pContainer_Inventory_Target = &m_Container_Inventory;
	}

	bool IsInsertable = (*pContainer_Inventory_Target)[_inventoryID].Check_IsItemInsertable_At(_index_Horizontal, _index_Vertical, _pItemData_Insert);
	return IsInsertable;
}



EHHM_InventoryReturn UHHM_Component_Inventory::Item_Insert_At(int32& _inventoryItemID_Return, UHHM_ItemData*& _pItemData_Insert, const bool& _isRoot, const int32& _inventoryID,
	const int32& _index_Horizontal, const int32& _index_Vertical)
{
	bool IsValidIndex = Check_IsValidIndex(_isRoot, _inventoryID, _index_Horizontal, _index_Vertical);
	if (IsValidIndex == false) {
		//Exception invalid inventory index
		return EHHM_InventoryReturn::Return_Error;
	}

	if (_pItemData_Insert == nullptr) {
		//Exception ItemData is nullptr
		return EHHM_InventoryReturn::Return_NoItem;
	}

	

	TMap<int32, FHHM_Inventory>* pContainer_Inventory_Target = nullptr;
	if (_isRoot) {
		pContainer_Inventory_Target = &m_Container_Inventory_Root;
	}
	else {
		pContainer_Inventory_Target = &m_Container_Inventory;
	}

	int32 InventoryItemID_Return = -1;
	EHHM_InventoryReturn InventoryReturn = (*pContainer_Inventory_Target)[_inventoryID].Item_Insert_At(InventoryItemID_Return, _pItemData_Insert, _index_Horizontal, _index_Vertical);
	if (InventoryReturn != EHHM_InventoryReturn::Return_Succeed) {
		//Exception Item insertion at target slot failed
		return InventoryReturn;
	}

	_inventoryItemID_Return = InventoryItemID_Return;
	return InventoryReturn;
}

EHHM_InventoryReturn UHHM_Component_Inventory::Item_Insert_AtInventory(int32& _inventoryItemID_Return, UHHM_ItemData*& _pItemData_Insert, const bool& _isRoot, const int32& _inventoryID) 
{
	bool IsValidInventoryID = Check_IsValidInventoryID(_isRoot, _inventoryID);
	if (IsValidInventoryID == false) {
		//Exception Invalid inventory ID
		return EHHM_InventoryReturn::Return_Error;
	}

	if (_pItemData_Insert == nullptr) {
		//Exception item data is nullptr
		return EHHM_InventoryReturn::Return_NoItem;
	}



	TMap<int32, FHHM_Inventory>* pContainer_Inventory_Target = nullptr;
	if (_isRoot) {
		pContainer_Inventory_Target = &m_Container_Inventory_Root;
	}
	else {
		pContainer_Inventory_Target = &m_Container_Inventory;
	}

	int32 InventoryItemID_Return = -1;
	EHHM_InventoryReturn InventoryReturn = (*pContainer_Inventory_Target)[_inventoryID].Item_Insert(InventoryItemID_Return, _pItemData_Insert);
	if (InventoryReturn != EHHM_InventoryReturn::Return_Succeed) {
		//Exception item insertion at target inventory failed
		return InventoryReturn;
	}

	_inventoryItemID_Return = InventoryItemID_Return;
	return InventoryReturn;
}

EHHM_InventoryReturn UHHM_Component_Inventory::Item_Insert(int32& _inventoryItemID_Return, int32& _inventoryID_Return, UHHM_ItemData*& _pItemData_Insert, const bool& _isRoot)
{
	if (_pItemData_Insert == nullptr) {
		//Exception item data is nullptr
		return EHHM_InventoryReturn::Return_NoItem;
	}

	

	TMap<int32, FHHM_Inventory>* pContainer_Inventory_Target = nullptr;
	if (_isRoot) {
		pContainer_Inventory_Target = &m_Container_Inventory_Root;
	}
	else {
		pContainer_Inventory_Target = &m_Container_Inventory;
	}



	int32 InventoryID_Return = -1;
	int32 InventoryItemID_Return = -1;
	EHHM_InventoryReturn InventoryReturn = EHHM_InventoryReturn::Return_End;

	for (auto& pair : *pContainer_Inventory_Target) {
		bool IsInsertable = pair.Value.Check_IsItemInsertable(_pItemData_Insert);
		if (IsInsertable == true) {
			InventoryReturn = pair.Value.Item_Insert(InventoryItemID_Return, _pItemData_Insert);
			if (InventoryReturn != EHHM_InventoryReturn::Return_Succeed) {
				//Exception Insert item failed
				return InventoryReturn;
			}

			InventoryID_Return = pair.Key;
			break;
		}
	}

	return InventoryReturn;
}



EHHM_InventoryReturn UHHM_Component_Inventory::Item_Pop_At(UHHM_ItemData*& _pItemData_Return, const bool& _isRoot, const int32& _inventoryID, const int32& _index_Horizontal, const int32& _index_Vertical)
{
	bool IsValidIndex = Check_IsValidIndex(_isRoot, _inventoryID, _index_Horizontal, _index_Vertical);
	if (IsValidIndex == false) {
		//Exception invalid inventory index
		return EHHM_InventoryReturn::Return_Error;
	}



	TMap<int32, FHHM_Inventory>* pContainer_Inventory_Target = nullptr;
	if (_isRoot) {
		pContainer_Inventory_Target = &m_Container_Inventory_Root;
	}
	else {
		pContainer_Inventory_Target = &m_Container_Inventory;
	}

	UHHM_ItemData* pItemData_Return = nullptr;
	EHHM_InventoryReturn InventoryReturn = (*pContainer_Inventory_Target)[_inventoryID].Item_Pop_At(pItemData_Return, _index_Horizontal, _index_Vertical);
	if (InventoryReturn != EHHM_InventoryReturn::Return_Succeed) {
		//Exception pop item from target inventory failed
		return InventoryReturn;
	}

	_pItemData_Return = pItemData_Return;
	return InventoryReturn;
}



int32 UHHM_Component_Inventory::Get_AvailableInventoryID(const bool& _isRoot)
{
	TMap<int32, FHHM_Inventory>* pContainer_Inventory_Target = nullptr;
	if (_isRoot) {
		pContainer_Inventory_Target = &m_Container_Inventory_Root;
	}
	else {
		pContainer_Inventory_Target = &m_Container_Inventory;
	}

	

	for (int32 index = 0; index < INVENTORY_HARD_LIMIT; ++index) {
		bool IsOccupied = pContainer_Inventory_Target->Contains(index);
		if (IsOccupied == false) {
			return index;
		}
	}

	return -1;
}



bool UHHM_Component_Inventory::Check_IsRoomFree(const bool& _isRoot, const int32& _inventoryID, const int32& _index_Horizontal, const int32& _index_Vertical, const int32& _size_Horizontal, const int32& _size_Vertical)
{
	bool IsValidIndex = Check_IsValidIndex(_isRoot, _inventoryID, _index_Horizontal, _index_Vertical);
	if (IsValidIndex == false) {
		//Exception invalid inventory index
		return false;
	}

	TMap<int32, FHHM_Inventory>* pContainer_Inventory_Target = nullptr;
	if (_isRoot) {
		pContainer_Inventory_Target = &m_Container_Inventory_Root;
	}
	else {
		pContainer_Inventory_Target = &m_Container_Inventory;
	}

	

	bool IsRoomFree = (*pContainer_Inventory_Target)[_inventoryID].Check_IsRoomFree(_index_Horizontal, _index_Vertical, _size_Horizontal, _size_Vertical);
	return IsRoomFree;
}

//EHHM_InventoryReturn UHHM_Component_Inventory::Find_FreeRoom_AtInventory(int32& _index_Return, const bool& _isRoot, const int32& _inventoryID, const int32& _size_Horizontal, const int32& _size_Vertical)
//{
//	bool IsValidInventoryID = Check_IsValidInventoryID(_isRoot, _inventoryID);
//	if (IsValidInventoryID == false) {
//		//Exception invalid inventory id
//		return EHHM_InventoryReturn::Return_Error;
//	}
//
//	TMap<int32, FHHM_Inventory>* pContainer_Inventory_Target = nullptr;
//	if (_isRoot) {
//		pContainer_Inventory_Target = &m_Container_Inventory_Root;
//	}
//	else {
//		pContainer_Inventory_Target = &m_Container_Inventory;
//	}
//
//
//
//	int32 Index
//}
//
//EHHM_InventoryReturn UHHM_Component_Inventory::Find_FreeRoom(int32& _index_Return, bool& _isRoot_Return, int32& _inventoryID_Return, const int32& _size_Horizontal, const int32& _size_Vertical)
//{
//	return EHHM_InventoryReturn();
//}



//void UHHM_Component_Inventory::CleanUp_At(int32 _index_Horizontal, int32 _index_Vertical)
//{
//	bool IsValidIndex = Check_IsValidIndex(_index_Horizontal, _index_Vertical);
//	if (IsValidIndex == false) {
//		//Exception Invalid Index
//		return;
//	}
//
//	bool IsSlotOccupied = m_InventorySlotData[_index_Vertical].SlotDataRow[_index_Horizontal].IsOccupied;
//	if (IsSlotOccupied == false) {
//		return;
//	}
//
//	int32 Index_Item = m_InventorySlotData[_index_Vertical].SlotDataRow[_index_Horizontal].ItemID_Occupied;
//
//	FIntPoint IndexPoint_LeftTop = FIntPoint::ZeroValue;
//	bool IsSucceed_FindFirstSlot = Find_FirstSlot_ItemOccupied(IndexPoint_LeftTop, Index_Item);
//	if (IsSucceed_FindFirstSlot == false) {
//		//Exception [Warning] Finding First slot failed.
//		return;
//	}
//
//	
//}

