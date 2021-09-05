#include "HHM_Inventory.h"

#include "Item/Base/HHM_Item.h"



FHHM_Inventory::FHHM_Inventory() {
	m_Container_ItemData.Empty();
	m_InventorySlotData.Empty();
}



bool FHHM_Inventory::Initialize_Inventory(uint32 _size_Horizontal, uint32 _size_Vertical)
{
	if (_size_Horizontal <= 0 || _size_Horizontal <= 0) {
		//HHM Exception : Unavailable inventory size
		return false;
	}

	//Initialize Column
	//// Note : 이니셜라이즈된 InventorySlotRow 데이터를 m_InventorySlotData.Init 에 인자로 넣어 처리할수도 있긴 하지만, 아무래도 내부에 리스트가 들어가는 만큼 같은 주소를 참조하는 리스트가
	//// 있을수도 있겠다는 생각에 일일이 하나하나 초기화하고 이니셜라이즈 하는 방법으로 진행했다. 언리얼에서 만든만큼 그런걱정은 안해도 되겠지만 혹시 모르니깐
	m_InventorySlotData.Init(FHHM_Data_Inventory_Slot_Row(), _size_Vertical);

	//Initialize Row
	FHHM_Data_Inventory_Slot Data_InventorySlot_ToAdd = FHHM_Data_Inventory_Slot();
	Data_InventorySlot_ToAdd.Reset();

	for (int32 _index_Row = 0; _index_Row < _size_Vertical; ++_index_Row) {
		m_InventorySlotData[_index_Row].SlotDataRow.Empty();
		m_InventorySlotData[_index_Row].SlotDataRow.Init(Data_InventorySlot_ToAdd, _size_Horizontal);
	}

	//Set Member Variables
	m_Size_Horizontal = _size_Horizontal;
	m_Size_Vertical = _size_Vertical;

	return true;
}

FIntPoint FHHM_Inventory::Get_InventorySize()
{
	FIntPoint InventorySize_Return = FIntPoint();
	InventorySize_Return.X = m_Size_Horizontal;
	InventorySize_Return.Y = m_Size_Vertical;

	return InventorySize_Return;
}

EHHM_InventoryReturn FHHM_Inventory::Get_ItemDataPtr_AtSlot(UHHM_ItemData*& _pItemData_Return, uint32 _index_Horizontal, uint32 _index_Vertical)
{
	//Check is input index valid
	bool IsAvailable_Index = Check_IsValidIndex(_index_Horizontal, _index_Vertical);
	if (IsAvailable_Index == false) {
		//Exception : Not Available Inventory Index
		return EHHM_InventoryReturn::Return_NoRoom;
	}



	//Get ItemData on target slot
	UHHM_ItemData* pItemData_Return = nullptr;

	int32 InventoryItemID_Return = m_InventorySlotData[_index_Vertical].SlotDataRow[_index_Horizontal].ItemID_Occupied;
	FHHM_Data_Inventory_Item* pInventoryItemData_Find = nullptr;
	pInventoryItemData_Find = m_Container_ItemData.Find(InventoryItemID_Return);
	if (pInventoryItemData_Find == nullptr) {
		//No itemdata on target slot
		return EHHM_InventoryReturn::Return_NoItem;
	}

	//Return found itemdata
	pItemData_Return = pInventoryItemData_Find->pItemData;
	_pItemData_Return = pItemData_Return;

	return EHHM_InventoryReturn::Return_Succeed;
}

TMap<int32, FHHM_Data_Inventory_Item>& FHHM_Inventory::Get_ItemDataContainer_Ref()
{
	return m_Container_ItemData;
}



bool FHHM_Inventory::Check_IsValidIndex(uint32 _index_Horizontal, uint32 _index_Vertical)
{
	return _index_Horizontal < 0 || _index_Horizontal >= m_Size_Horizontal
		|| _index_Vertical < 0 || _index_Vertical >= m_Size_Vertical ? false : true;
}

bool FHHM_Inventory::Check_IsItemSwappable(int32 _index_Horizontal, int32 _index_Vertical, UHHM_ItemData*& _pItemData_Swap)
{
	//Valid Check
	//Input ItemData Check
	if (_pItemData_Swap == nullptr) {
		//Exception Null input
		return false;
	}

	//Input Index Check
	bool IsValid_Index = Check_IsValidIndex(_index_Horizontal, _index_Vertical);
	if (IsValid_Index == false) {
		//Exception Invalid Index
		return false;
	}



	//Is target slot occupied by an item?
	bool IsOccupied = m_InventorySlotData[_index_Vertical].SlotDataRow[_index_Horizontal].IsOccupied;
	if (IsOccupied == false) {
		//Exception Target slot not occupied
		return false;
	}



	//Valid Check for occupying item
	int32 InventoryItemID_Target = m_InventorySlotData[_index_Vertical].SlotDataRow[_index_Horizontal].ItemID_Occupied;
	bool IsContains_InventoryItemID = m_Container_ItemData.Contains(InventoryItemID_Target);
	if (IsContains_InventoryItemID == false) {
		//Exception Occupied slot has invalid inventoryItemID. Possible to corrupted inventory data
		return false;
	}
	if (m_Container_ItemData[InventoryItemID_Target].pItemData == nullptr) {
		//Exception inventory contains NULL itemdata. possible to corrupted inventory data
		return false;
	}



	//Get source slot of occupying item
	FIntPoint IndexPoint_Source = FIntPoint(-1, -1);
	if (m_InventorySlotData[_index_Vertical].SlotDataRow[_index_Horizontal].IsSource) {
		IndexPoint_Source = FIntPoint(_index_Horizontal, _index_Vertical);
	}
	else {
		int32 Num_OccupyingSlot = m_Container_ItemData[InventoryItemID_Target].Container_IndexPoint_Occupied.Num();
		if (Num_OccupyingSlot <= 0) {
			//Exception InventoryItemData has 0 or even below number of occupying slot. Possible to corrupted inventory data
			return false;
		}

		for (int32 index_Slot = 0; index_Slot < Num_OccupyingSlot; ++index_Slot) {
			FIntPoint IndexPoint_Search = m_Container_ItemData[InventoryItemID_Target].Container_IndexPoint_Occupied[index_Slot];
			if (m_InventorySlotData[IndexPoint_Search.Y].SlotDataRow[IndexPoint_Search.X].IsSource) {
				//Source IndexPoint Found
				IndexPoint_Source = IndexPoint_Search;
				break;
			}
		}

		//Is Source IndexPoint Found?
		if (IndexPoint_Source.X < 0 || IndexPoint_Source.Y < 0) {
			//Exception Can not find source slot of target item. Possible to corrupted inventory data
			return false;
		}
	}



	//Get Item
	UHHM_Item* pItem = m_Container_ItemData[InventoryItemID_Target].pItemData->Get_Item();
	if (pItem == nullptr) {
		//Exception ItemData has no reference to Actual Item class. Possible to corrupted ItemData or not fully completed ItemData Class
		return false;
	}

	//Get ItemSize
	FIntPoint Size_Item = pItem->Get_ItemSize();
	FIntPoint IndexPoint_SourceEnd = IndexPoint_Source + Size_Item - FIntPoint(1, 1);

	//Valid Check of occupying slots of target item
	for (int32 index_Vertical_ValidCheck = IndexPoint_Source.Y; index_Vertical_ValidCheck <= IndexPoint_SourceEnd.Y; ++index_Vertical_ValidCheck) {
		for (int32 index_Horizontal_ValidCheck = IndexPoint_Source.X; index_Horizontal_ValidCheck <= IndexPoint_SourceEnd.X; ++index_Vertical_ValidCheck) {
			bool IsOccupied = m_InventorySlotData[index_Vertical_ValidCheck].SlotDataRow[index_Horizontal_ValidCheck].IsOccupied;
			if (IsOccupied == false) {
				//Exception InventoryItemData's occupying slot data is incorrect. Possible to corrupted inventory data
				return false;
			}

			int32 InventoryItemID_Check = m_InventorySlotData[index_Vertical_ValidCheck].SlotDataRow[index_Horizontal_ValidCheck].ItemID_Occupied;
			if (InventoryItemID_Check != InventoryItemID_Target) {
				//Exception The occupied slots of target item is occupied by other inventoryItemID. Possible to corrupted inventory data. Item slot may occupied by multiple items
				return false;
			}
		}
	}

	//Get ItemSize of swapping item(input item)
	UHHM_Item* pItem_Swap = _pItemData_Swap->Get_Item();
	if (pItem_Swap == nullptr) {
		//Exception Input itemData has no reference to actual item. Possible to corrupted ItemData or incompleted itemData class
		return false;
	}

	//Get needed room for swapping item(input item)
	FIntPoint Size_Item_Swap = pItem_Swap->Get_ItemSize();
	FIntPoint IndexPoint_SwapEnd = FIntPoint(_index_Horizontal, _index_Vertical) + Size_Item_Swap - FIntPoint(1, 1);



	//Check is item swappable
	if (IndexPoint_SwapEnd.X > IndexPoint_SourceEnd.X || IndexPoint_SwapEnd.Y > IndexPoint_SourceEnd.Y) {
		//Can't fit target slot
		return false;
	}



	return true;
}

bool FHHM_Inventory::Check_IsItemInsertable(UHHM_ItemData*& _pItemData)
{
	return false;
}

bool FHHM_Inventory::Check_IsItemInsertable_At(int32 _index_Horizontal, int32 _index_Vertical, UHHM_ItemData*& _pItemData_Insert)
{
	return false;
}



EHHM_InventoryReturn FHHM_Inventory::Item_Insert(int32& _InventoryItemID_Return, UHHM_ItemData*& _pItemData)
{
	return EHHM_InventoryReturn();
}

EHHM_InventoryReturn FHHM_Inventory::Item_Insert_At(int32& _InventoryItemID_Return, UHHM_ItemData*& _pItemData, const int32& _index_Horizontal, const int32& _index_Vertical)
{
	return EHHM_InventoryReturn();
}

EHHM_InventoryReturn FHHM_Inventory::Item_Pop_At(UHHM_ItemData*& _pItemData_Return, const int32& _index_Horizontal, const int32& _index_Vertical)
{
	return EHHM_InventoryReturn();
}

EHHM_InventoryReturn FHHM_Inventory::Item_Remove(UHHM_ItemData*& _pItemData_Remove)
{
	return EHHM_InventoryReturn();
}



FIntPoint FHHM_Inventory::Convert_IndexToIndexPoint(const int32& _index)
{
	return FIntPoint();
}

int32 FHHM_Inventory::Convert_IndexPointToIndex(const FIntPoint& _indexPoint)
{
	return int32();
}

bool FHHM_Inventory::Check_IsRoomFree(const int32& _index_Horizontal, const int32& _index_Vertical, const int32& _size_Horizontal, const int32& _size_Vertical)
{
	//Index Valid check
	bool IsValid_Index = Check_IsValidIndex(_index_Horizontal, _index_Vertical);
	if (IsValid_Index == false) {
		//Exception Invalid input index
		return false;
	}

	//Size valid check
	if (_size_Horizontal <= 0 || _size_Vertical <= 0) {
		//Exception Zero or even below item size
		return false;
	}

	//Get End indexPoint And check whether that point is valid or not
	int32 Index_Horizontal_End = _index_Horizontal + _size_Horizontal - 1;
	int32 Index_Vertical_End = _index_Vertical + _size_Vertical - 1;
	bool IsValid_Index_End = Check_IsValidIndex(Index_Horizontal_End, Index_Vertical_End);
	if (IsValid_Index_End == false) {
		return false;
	}

	

	//Check the room
	for (int32 index_Search_Vertical = _index_Vertical; index_Search_Vertical < Index_Vertical_End; ++index_Search_Vertical) {
		for (int32 index_Search_Horizontal = _index_Horizontal; index_Search_Horizontal < Index_Horizontal_End; ++index_Search_Horizontal) {
			bool IsSlotOccupied = m_InventorySlotData[index_Search_Vertical].SlotDataRow[index_Search_Horizontal].IsOccupied;
			if (IsSlotOccupied == true) {
				//Room Is Not Free
				return false;
			}
		}
	}

	return true;
}

int32 FHHM_Inventory::Find_FreeRoom(const int32& _size_Horizontal, const int32& _size_Vertical)
{
	return int32();
}

int32 FHHM_Inventory::Find_ValidInventoryItemID()
{
	return int32();
}

bool FHHM_Inventory::Find_SourceIndex(FIntPoint& _indexPoint_Return, const int32 _index_InventoryItemKey)
{
	return false;
}

bool FHHM_Inventory::Set_RoomOccupied(const int32& _index_Horizontal, const int32& _index_Vertical, const int32& _size_Horizontal, const int32& _size_Vertical, const int32& _inventoryItemID, FHHM_Data_Inventory_Item* _inventoryItemData)
{
	return false;
}

void FHHM_Inventory::Set_RoomFree(const int32& _inventoryItemID)
{
}

void FHHM_Inventory::Set_RoomFree_At(const int32& _index_Horizontal, const int32& _index_Vertical, const int32& _size_Horizontal, const int32& _size_Vertical, const bool& _bForceFree)
{
}
