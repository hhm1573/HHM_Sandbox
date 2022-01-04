#include "HHM_Inventory.h"

#include "Item/Base/HHM_Item.h"



constexpr int32 INVENTORY_ITEM_HARD_LIMIT = 1000;



FHHM_Inventory::FHHM_Inventory() {
	m_Container_ItemData.Empty();
	m_InventorySlotData.Empty();
}



bool FHHM_Inventory::Initialize_Inventory(const FHHM_Data_Inventory& _data_Inventory)
{
	if (_data_Inventory.m_Size_Horizontal <= 0 || _data_Inventory.m_Size_Vertical <= 0) {
		//HHM Exception : Unavailable inventory size
		return false;
	}

	//Initialize Column
	//// Note : 이니셜라이즈된 InventorySlotRow 데이터를 m_InventorySlotData.Init 에 인자로 넣어 처리할수도 있긴 하지만, 아무래도 내부에 리스트가 들어가는 만큼 같은 주소를 참조하는 리스트가
	//// 있을수도 있겠다는 생각에 일일이 하나하나 초기화하고 이니셜라이즈 하는 방법으로 진행했다. 언리얼에서 만든만큼 그런걱정은 안해도 되겠지만 혹시 모르니깐
	m_InventorySlotData.Init(FHHM_Data_Inventory_Slot_Row(), _data_Inventory.m_Size_Vertical);

	//Initialize Row
	FHHM_Data_Inventory_Slot Data_InventorySlot_ToAdd = FHHM_Data_Inventory_Slot();
	Data_InventorySlot_ToAdd.Reset();

	for (int32 _index_Row = 0; _index_Row < _data_Inventory.m_Size_Vertical; ++_index_Row) {
		m_InventorySlotData[_index_Row].SlotDataRow.Empty();
		m_InventorySlotData[_index_Row].SlotDataRow.Init(Data_InventorySlot_ToAdd, _data_Inventory.m_Size_Horizontal);
	}

	//Set Member Variables
	m_Data_Inventory = _data_Inventory;

	return true;
}

//TMap<int32, FHHM_Data_Inventory_Item>& FHHM_Inventory::BP_Get_Container_ItemData()
//{
//	return m_Container_ItemData;
//}

FIntPoint FHHM_Inventory::Get_InventorySize() const
{
	FIntPoint InventorySize_Return = FIntPoint();
	InventorySize_Return.X = m_Data_Inventory.m_Size_Horizontal;
	InventorySize_Return.Y = m_Data_Inventory.m_Size_Vertical;

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



bool FHHM_Inventory::Check_IsValidIndex(const int32& _index_Horizontal, const int32& _index_Vertical) const 
{
	return _index_Horizontal < 0 || _index_Horizontal >= m_Data_Inventory.m_Size_Horizontal
		|| _index_Vertical < 0 || _index_Vertical >= m_Data_Inventory.m_Size_Vertical ? false : true;
}

bool FHHM_Inventory::Check_IsItemSwappable(const int32& _index_Horizontal, const int32& _index_Vertical, const UHHM_ItemData*& _pItemData_Swap) const 
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
		int32 Num_OccupyingSlot = m_Container_ItemData[InventoryItemID_Target].Container_Index_Occupied.Num();
		if (Num_OccupyingSlot <= 0) {
			//Exception InventoryItemData has 0 or even below number of occupying slot. Possible to corrupted inventory data
			return false;
		}

		for (int32 index_Slot = 0; index_Slot < Num_OccupyingSlot; ++index_Slot) {
			int32 Index_Search = m_Container_ItemData[InventoryItemID_Target].Container_Index_Occupied[index_Slot];
			FIntPoint IndexPoint_Search = Convert_IndexToIndexPoint(Index_Search);
			if (IndexPoint_Search.X < 0 || IndexPoint_Search.Y < 0) {
				//Exception convert index to index point failed
				return false;
			}

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



	//Get ItemSize
	FIntPoint Size_Item = m_Container_ItemData[InventoryItemID_Target].pItemData->Get_ItemSize();
	FIntPoint IndexPoint_SourceEnd = IndexPoint_Source + Size_Item - FIntPoint(1, 1);

	//Valid Check of occupying slots of target item
	for (int32 index_Vertical_ValidCheck = IndexPoint_Source.Y; index_Vertical_ValidCheck <= IndexPoint_SourceEnd.Y; ++index_Vertical_ValidCheck) {
		for (int32 index_Horizontal_ValidCheck = IndexPoint_Source.X; index_Horizontal_ValidCheck <= IndexPoint_SourceEnd.X; ++index_Vertical_ValidCheck) {
			bool IsOccupied_Slot = m_InventorySlotData[index_Vertical_ValidCheck].SlotDataRow[index_Horizontal_ValidCheck].IsOccupied;
			if (IsOccupied_Slot == false) {
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

	//Get needed room for swapping item(input item)
	FIntPoint Size_Item_Swap = _pItemData_Swap->Get_ItemSize();
	FIntPoint IndexPoint_SwapEnd = FIntPoint(_index_Horizontal, _index_Vertical) + Size_Item_Swap - FIntPoint(1, 1);



	//Check is item swappable
	if (IndexPoint_SwapEnd.X > IndexPoint_SourceEnd.X || IndexPoint_SwapEnd.Y > IndexPoint_SourceEnd.Y) {
		//Can't fit target slot
		return false;
	}



	return true;
}

bool FHHM_Inventory::Check_IsItemInsertable(UHHM_ItemData*& _pItemData) const 
{
	if (_pItemData == nullptr) {
		//Exception ItemData is nullptr
		return false;
	}

	FIntPoint ItemSize = _pItemData->Get_ItemSize();
	if (ItemSize.X <= 0 || ItemSize.Y <= 0) {
		//Exception Unavailable ItemSize
		return false;
	}

	int32 Index_InsertableSlot = Find_FreeRoom(ItemSize.X, ItemSize.Y);
	if (Index_InsertableSlot < 0) {
		//No Insertable Slot
		return false;
	}

	return true;
}

bool FHHM_Inventory::Check_IsItemInsertable_At(const int32& _index_Horizontal, const int32& _index_Vertical, UHHM_ItemData*& _pItemData_Insert) const 
{
	//Valid check
	if (_index_Horizontal < 0 || _index_Vertical < 0) {
		//Exception Negative index value
		return false;
	}

	if (_pItemData_Insert == nullptr) {
		//Exception itemData is nullptr
		return false;
	}



	FIntPoint Size_Item = _pItemData_Insert->Get_ItemSize();
	if (Size_Item.X <= 0 || Size_Item.Y <= 0) {
		//Exception unavailable item size
		return false;
	}

	

	//Check Room
	bool IsRoomFree = Check_IsRoomFree(_index_Horizontal, _index_Vertical, Size_Item.X, Size_Item.Y);
	return IsRoomFree;
}



EHHM_InventoryReturn FHHM_Inventory::Item_Insert(int32& _InventoryItemID_Return, UHHM_ItemData*& _pItemData)
{
	//Valid check
	if (_pItemData == nullptr) {
		//Exception ItemData is nullptr
		return EHHM_InventoryReturn::Return_NoItem;
	}

	

	FIntPoint Size_Item = _pItemData->Get_ItemSize();
	if (Size_Item.X <= 0 || Size_Item.Y <= 0) {
		//Exception Unavailable item size
		return EHHM_InventoryReturn::Return_Error;
	}



	//Find Room
	int32 Index_Found = Find_FreeRoom(Size_Item.X, Size_Item.Y);
	if (Index_Found < 0) {
		//No Room Found
		return EHHM_InventoryReturn::Return_NoRoom;
	}

	//Convert index to indexPoint
	FIntPoint IndexPoint = Convert_IndexToIndexPoint(Index_Found);
	if (IndexPoint.X < 0 || IndexPoint.Y < 0) {
		//Excetpion Convert index point failed
		return EHHM_InventoryReturn::Return_Error;
	}



	//Insert Item
	EHHM_InventoryReturn Return = Item_Insert_At(_InventoryItemID_Return, _pItemData, IndexPoint.X, IndexPoint.Y);
	return Return;

	//Note : Insert_At 함수 등에서 같은 유효검사를 여러번 반복하기 때문에 비효율적이긴 하지만, 귀찮기도 하고 인벤토리 함수들은 엔티티나 타일등 보다 상대적으로 덜 쓰일것이라 생각이 되기에
	//그냥 내버려 두기로 하였다.
}

EHHM_InventoryReturn FHHM_Inventory::Item_Insert_At(int32& _InventoryItemID_Return, UHHM_ItemData*& _pItemData, const int32& _index_Horizontal, const int32& _index_Vertical)
{
	//Valid Check
	if (_pItemData == nullptr) {
		//Exception Item data is nullptr
		return EHHM_InventoryReturn::Return_NoItem;
	}

	bool IsValidIndex = Check_IsValidIndex(_index_Horizontal, _index_Vertical);
	if (IsValidIndex == false) {
		//Exception Invalid index
		return EHHM_InventoryReturn::Return_Error;
	}



	FIntPoint Size_Item = _pItemData->Get_ItemSize();
	if (Size_Item.X <= 0 || Size_Item.Y <= 0) {
		//Exception Unavailable Item size
		return EHHM_InventoryReturn::Return_Error;
	}



	//Check is room free
	bool IsRoomFree = Check_IsRoomFree(_index_Horizontal, _index_Vertical, Size_Item.X, Size_Item.Y);
	if (IsRoomFree == false) {
		//Can not insert item at target slot
		return EHHM_InventoryReturn::Return_NoRoom;
	}

	//Get Inventory Item ID
	int32 InventoryItemID_Found = Find_ValidInventoryItemID();
	if (InventoryItemID_Found < 0) {
		//Exception Can not find valid inventory item id
		return EHHM_InventoryReturn::Return_Error;
	}



	//Insert Item
	m_Container_ItemData.Add(InventoryItemID_Found, FHHM_Data_Inventory_Item(_pItemData));
	bool IsSucceed_OccupyRoom = Set_RoomOccupied(_index_Horizontal, _index_Vertical, Size_Item.X, Size_Item.Y, InventoryItemID_Found, &(m_Container_ItemData[InventoryItemID_Found]));
	if (IsSucceed_OccupyRoom == false) {
		//Exception Occupying room failed
		m_Container_ItemData.Remove(InventoryItemID_Found);
		return EHHM_InventoryReturn::Return_Error;
	}

	//Set source slot
	m_InventorySlotData[_index_Vertical].SlotDataRow[_index_Horizontal].Set_Source(true);

	return EHHM_InventoryReturn::Return_Succeed;
}

// HHM Note : [HHM To Do] 나중에 로깅과정을 넣게되었을 때 아이템 데이터에 문제있는 부분에서는 해당 데이터를 지워버리게끔 수정하기.

EHHM_InventoryReturn FHHM_Inventory::Item_Pop_At(UHHM_ItemData*& _pItemData_Return, const int32& _index_Horizontal, const int32& _index_Vertical)
{
	//Valid check
	bool IsValidIndex = Check_IsValidIndex(_index_Horizontal, _index_Vertical);
	if (IsValidIndex == false) {
		//Exception Invalid index
		return EHHM_InventoryReturn::Return_Error;
	}

	bool IsSlotOccupied = m_InventorySlotData[_index_Vertical].SlotDataRow[_index_Horizontal].IsOccupied;
	int32 InventoryItemID_Target = m_InventorySlotData[_index_Vertical].SlotDataRow[_index_Horizontal].ItemID_Occupied;
	if (IsSlotOccupied == false || InventoryItemID_Target < 0) {
		//Exception No Item
		return EHHM_InventoryReturn::Return_NoItem;
	}

	FHHM_Data_Inventory_Item* pInventoryItemData_Found = m_Container_ItemData.Find(InventoryItemID_Target);
	if (pInventoryItemData_Found == nullptr) {
		//Exception Target Inventory item data is nullptr
		return EHHM_InventoryReturn::Return_Error;
	}

	if (pInventoryItemData_Found->pItemData == nullptr) {
		//Exception Target InventoryItemData's ItemData is nullptr
		return EHHM_InventoryReturn::Return_Error;
	}



	//Remove Item data
	_pItemData_Return = pInventoryItemData_Found->pItemData;
	Set_RoomFree(InventoryItemID_Target);
	m_Container_ItemData.Remove(InventoryItemID_Target);

	return EHHM_InventoryReturn::Return_Succeed;
}

//Note : 딱히 필요 없을 것 같아서 구현 안해놓았음. 추후 기능이 필요해지면 구현. 외부에서 아이템 데이터를 가진상태로 아이템을 지우게 할일은 별로 없을듯.
EHHM_InventoryReturn FHHM_Inventory::Item_Remove(UHHM_ItemData*& _pItemData_Remove)
{
	return EHHM_InventoryReturn();
}



void FHHM_Inventory::Clear_Inventory()
{
	m_Container_ItemData.Empty();
	
	for (int32 index_Clear_Vertical = 0; index_Clear_Vertical < m_Data_Inventory.m_Size_Vertical; ++index_Clear_Vertical) {
		for (int32 index_Clear_Horizontal = 0; index_Clear_Horizontal < m_Data_Inventory.m_Size_Horizontal; ++index_Clear_Horizontal) {
			m_InventorySlotData[index_Clear_Vertical].SlotDataRow[index_Clear_Horizontal].Reset();
		}
	}
}



FIntPoint FHHM_Inventory::Convert_IndexToIndexPoint(const int32& _index) const 
{
	if (m_Data_Inventory.m_Size_Horizontal <= 0) {
		//Exception inventory size is not set
		return FIntPoint(-1, -1);
	}

	int32 Index_Horizontal = _index % m_Data_Inventory.m_Size_Horizontal;
	int32 Index_Vertical = _index / m_Data_Inventory.m_Size_Horizontal;
	FIntPoint IndexPoint_Return = FIntPoint(Index_Horizontal, Index_Vertical);

	return IndexPoint_Return;
}

int32 FHHM_Inventory::Convert_IndexPointToIndex(const FIntPoint& _indexPoint) const 
{
	if (m_Data_Inventory.m_Size_Horizontal <= 0 || m_Data_Inventory.m_Size_Vertical <= 0) {
		//Exception Inventory Size is not set properly
		return -1;
	}

	int32 Index_Return = _indexPoint.X + (_indexPoint.Y * m_Data_Inventory.m_Size_Horizontal);
	return Index_Return;
}

bool FHHM_Inventory::Check_IsRoomFree(const int32& _index_Horizontal, const int32& _index_Vertical, const int32& _size_Horizontal, const int32& _size_Vertical) const 
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

int32 FHHM_Inventory::Find_FreeRoom(const int32& _size_Horizontal, const int32& _size_Vertical) const 
{
	//valid check input value
	if (_size_Horizontal <= 0 || _size_Vertical <= 0) {
		//Exception input item size invalid
		return -1;
	}

	//inventory size check
	if (m_Data_Inventory.m_Size_Horizontal <= 0 || m_Data_Inventory.m_Size_Vertical <= 0) {
		//Exception inventory size is not properly set
		return -1;
	}
	
	if (_size_Horizontal > m_Data_Inventory.m_Size_Horizontal || _size_Vertical > m_Data_Inventory.m_Size_Vertical) {
		//Exception item is too big for inventory
		return -1;
	}



	//Get Possibly available slot radius By get last available index which closest to end of inventory
	int32 Index_Horizontal_Last = m_Data_Inventory.m_Size_Horizontal - _size_Horizontal;		//가장 끝에 가까운 인덱스. 가로4칸 인벤토리에 2칸짜리 아이템일 경우 4-2=2, index가 2인 슬롯은 3번째 칸 이므로 3,4칸을 차지
	int32 Index_Vertical_Last = m_Data_Inventory.m_Size_Vertical -_size_Vertical;
	


	//Search the room
	bool IsFindRoom = false;
	int32 Index_Horizontal_Find = -1;
	int32 Index_Vertical_Find = -1;

	for (int32 index_Vertical_Search = 0; index_Vertical_Search <= Index_Vertical_Last; ++index_Vertical_Search) {
		for (int32 index_Horizontal_Search = 0; index_Horizontal_Search <= Index_Horizontal_Last; ++index_Horizontal_Search) {

			bool IsRoomOccupied = false;

			for (int32 index_Vertical_Size = 0; index_Vertical_Size < _size_Vertical; ++index_Vertical_Size) {
				int32 Index_Vertical_OccupyCheck = index_Vertical_Search + index_Vertical_Size;
				for (int32 index_Horizontal_Size = 0; index_Horizontal_Size < _size_Horizontal; ++index_Horizontal_Size) {
					int32 Index_Horizontal_OccupyCheck = index_Horizontal_Search + index_Horizontal_Size;

					bool IsOccupied = m_InventorySlotData[Index_Vertical_OccupyCheck].SlotDataRow[Index_Horizontal_OccupyCheck].IsOccupied;
					if (IsOccupied == true) {
						IsRoomOccupied = true;
						break;
					}
				}
				
				if (IsRoomOccupied == true) {
					break;
				}

			}

			if (IsRoomOccupied == false) {
				IsFindRoom = true;
				Index_Horizontal_Find = index_Horizontal_Search;
				Index_Vertical_Find = index_Vertical_Search;

				break;
			}

		}

		if (IsFindRoom == true) {
			break;
		}

	}

	if (IsFindRoom == false) {
		//Can not find free room for item
		return -1;
	}



	//Return Find Slot
	int32 Index_Return = Convert_IndexPointToIndex(FIntPoint(Index_Horizontal_Find, Index_Vertical_Find));
	if (Index_Return < 0) {
		//Exception Convert failed
		return -1;
	}

	return Index_Return;
}

int32 FHHM_Inventory::Find_ValidInventoryItemID() const 
{
	for (int32 index_Search = 0; index_Search < INVENTORY_ITEM_HARD_LIMIT; ++index_Search) {
		bool IsOccupied_ID = m_Container_ItemData.Contains(index_Search);
		if (IsOccupied_ID == false) {
			return index_Search;
		}
	}

	return -1;
}

EHHM_InventoryReturn FHHM_Inventory::Find_SourceIndex(FIntPoint& _indexPoint_Return, const int32 _index_InventoryItemID) 
{
	//Valid check
	if (_index_InventoryItemID < 0) {
		//Invalid InventoryItemID
		return EHHM_InventoryReturn::Return_Error;
	}

	FHHM_Data_Inventory_Item* pInventoryItemData_Found = m_Container_ItemData.Find(_index_InventoryItemID);
	if (pInventoryItemData_Found == nullptr) {
		//Exception Can not find InventoryItemData
		return EHHM_InventoryReturn::Return_Error;
	}



	//Search Source Slot
	int32 Num_OccupyingSlot = pInventoryItemData_Found->Container_Index_Occupied.Num();
	for (int32 _index_Search = 0; _index_Search < Num_OccupyingSlot; ++_index_Search) {
		int32 Index_Slot = pInventoryItemData_Found->Container_Index_Occupied[_index_Search];
		FIntPoint IndexPoint_Search = Convert_IndexToIndexPoint(Index_Slot);
		if (IndexPoint_Search.X < 0 || IndexPoint_Search.Y < 0) {
			//Exception Convert Failed
			return EHHM_InventoryReturn::Return_Error;
		}

		bool IsSource = m_InventorySlotData[IndexPoint_Search.Y].SlotDataRow[IndexPoint_Search.X].IsSource;
		if (IsSource == true) {
			//Source Found
			_indexPoint_Return = IndexPoint_Search;
			return EHHM_InventoryReturn::Return_Succeed;
		}
	}



	//Can not find source
	return EHHM_InventoryReturn::Return_Error;
}

bool FHHM_Inventory::Set_RoomOccupied(const int32& _index_Horizontal, const int32& _index_Vertical, const int32& _size_Horizontal, const int32& _size_Vertical, const int32& _inventoryItemID, FHHM_Data_Inventory_Item* _pInventoryItemData)
{
	//Valid check
	if (_index_Horizontal < 0 || _index_Vertical < 0) {
		//Exception Zero or below index
		return false;
	}

	bool IsValidIndex = Check_IsValidIndex(_index_Horizontal, _index_Vertical);
	if (IsValidIndex == false) {
		//Exception invalid index
		return false;
	}

	if (_size_Horizontal <= 0 || _size_Vertical <= 0) {
		//Exception invalid item size
		return false;
	}

	if (_inventoryItemID < 0) {
		//Exception Invalid InventoryItemID
		return false;
	}

	if (_pInventoryItemData == nullptr) {
		//Exception InventoryItemData is nullptr
		return false;
	}

	//Check whether item will fit in inventory or too big to go through out of inventory
	int32 Index_Horizontal_ItemEnd = _index_Horizontal + _size_Horizontal - 1;
	int32 Index_Vertical_ItemEnd = _index_Vertical + _size_Vertical - 1;
	if (Index_Horizontal_ItemEnd >= m_Data_Inventory.m_Size_Horizontal || Index_Vertical_ItemEnd >= m_Data_Inventory.m_Size_Vertical) {
		//Exception Inventory Can not fit on inventory's target slot
		return false;
	}



	//Check if there is occupied slot in room
	for (int32 index_Vertical_Check = _index_Vertical; index_Vertical_Check <= Index_Vertical_ItemEnd; ++index_Vertical_Check) {
		for (int32 index_Horizontal_Check = _index_Horizontal; index_Horizontal_Check <= Index_Horizontal_ItemEnd; ++index_Horizontal_Check) {
			if (m_InventorySlotData[index_Vertical_Check].SlotDataRow[index_Horizontal_Check].IsOccupied == true) {
				//Exception there is occupied slot in room
				return false;
			}
		}
	}



	//Set Occupy
	for (int32 index_Vertical_Occupy = _index_Vertical; index_Vertical_Occupy <= Index_Vertical_ItemEnd; ++index_Vertical_Occupy) {
		for (int32 index_Horizontal_Occupy = _index_Horizontal; index_Horizontal_Occupy <= Index_Horizontal_ItemEnd; ++index_Horizontal_Occupy) {

			m_InventorySlotData[index_Vertical_Occupy].SlotDataRow[index_Horizontal_Occupy].Set_Occupied(_inventoryItemID);
			int32 Index_Occupy = Convert_IndexPointToIndex(FIntPoint(index_Horizontal_Occupy, index_Vertical_Occupy));
			if (Index_Occupy < 0) {
				//Exception Index convertion failed
				Set_RoomFree_At(_index_Horizontal, _index_Vertical, _size_Horizontal, _size_Vertical, true);
				return false;
			}
			_pInventoryItemData->Container_Index_Occupied.Add(Index_Occupy);

		}
	}

	//Set Source
	m_InventorySlotData[_index_Vertical].SlotDataRow[_index_Horizontal].Set_Source(true);

	return true;

}

void FHHM_Inventory::Set_RoomFree(const int32& _inventoryItemID)
{
	//Valid check
	if (_inventoryItemID < 0) {
		//Exception Negative InventoryItemID
		return;
	}

	FHHM_Data_Inventory_Item* pInventoryItemData_Found = nullptr;
	pInventoryItemData_Found = m_Container_ItemData.Find(_inventoryItemID);
	if (pInventoryItemData_Found) {
		//Exception No InventoryItemData on target inventoryItemID
		return;
	}

	

	if (pInventoryItemData_Found->pItemData == nullptr) {
		//Exception InventoryItemData has NULL itemData
		return;
	}

	

	//Free Room
	FIntPoint IntPoint_Source = FIntPoint::ZeroValue;
	EHHM_InventoryReturn Return_FindSource = Find_SourceIndex(IntPoint_Source, _inventoryItemID);
	if (Return_FindSource != EHHM_InventoryReturn::Return_Succeed) {
		//Exception Can not find source index
		return;
	}

	FIntPoint Size_Item = pInventoryItemData_Found->pItemData->Get_ItemSize();
	if (Size_Item.X <= 0 || Size_Item.Y <= 0) {
		//Exception invalid item size
		return;
	}

	Set_RoomFree_At(IntPoint_Source.X, IntPoint_Source.Y, Size_Item.X, Size_Item.Y);

}

void FHHM_Inventory::Set_RoomFree_At(const int32& _index_Horizontal, const int32& _index_Vertical, const int32& _size_Horizontal, const int32& _size_Vertical, const bool& _bForceFree)
{
	//if ForceFree is set as true, do valid check on every index and set free if valid.
	//so it might take much more resource than Non-ForceFree

	FIntPoint SlotToFree_LeftTop = FIntPoint(_index_Horizontal, _index_Vertical);
	FIntPoint SlotToFree_RightBottom = SlotToFree_LeftTop;
	SlotToFree_RightBottom.X = SlotToFree_LeftTop.X + _size_Horizontal - 1;
	SlotToFree_RightBottom.Y = SlotToFree_LeftTop.Y + _size_Vertical - 1;

	if (_bForceFree) {
		for (int32 index_Free_Vertical = SlotToFree_LeftTop.Y; index_Free_Vertical <= SlotToFree_RightBottom.Y; ++index_Free_Vertical) {
			for (int32 index_Free_Horizontal = SlotToFree_LeftTop.X; index_Free_Horizontal <= SlotToFree_RightBottom.X; ++index_Free_Horizontal) {
				bool IsValidIndex = Check_IsValidIndex(index_Free_Horizontal, index_Free_Vertical);
				if (IsValidIndex == true) {
					m_InventorySlotData[index_Free_Vertical].SlotDataRow[index_Free_Horizontal].Reset();
				}
			}
		}
	}
	else {
		//Valid Check

		if (_index_Horizontal < 0 || _index_Vertical < 0) {
			//Note : 이부분은 지워도 무관함. 사이즈가 음수가 나와 우측하단 위치가 좌상단으로 오게될 순 있지만 시작 인덱스 유효검사는 어차피 따로 실행하므로 음수인지 검사를 할 필요는 없음.
			// 그냥 이왕 검사하는거 보기좋게 가능한건 다 검사하자고 넣었을 뿐.
			//Exception Negative SlotIndex
			return;
		}

		if (_size_Horizontal <= 0 || _size_Vertical <= 0) {
			//Exception Unavailable item size
			return;
		}

		bool IsValid_Index_LeftTop = Check_IsValidIndex(SlotToFree_LeftTop.X, SlotToFree_LeftTop.Y);
		if (IsValid_Index_LeftTop == false) {
			//Exception Invalid Index
			return;
		}

		bool IsValid_Index_RightBottom = Check_IsValidIndex(SlotToFree_RightBottom.X, SlotToFree_RightBottom.Y);
		if (IsValid_Index_RightBottom == false) {
			//Exception Invalid Index
			return;
		}



		//Check if multiple item overlapping on target room
		int32 InventoryItemID_LeftTop = m_InventorySlotData[SlotToFree_LeftTop.Y].SlotDataRow[SlotToFree_LeftTop.X].ItemID_Occupied;
		for (int32 index_Check_Vertical = SlotToFree_LeftTop.Y; index_Check_Vertical <= SlotToFree_RightBottom.Y; ++index_Check_Vertical) {
			for (int32 index_Check_Horizontal = SlotToFree_LeftTop.X; index_Check_Horizontal <= SlotToFree_RightBottom.X; ++index_Check_Horizontal) {
				int32 InventoryItemID_Check = m_InventorySlotData[index_Check_Vertical].SlotDataRow[index_Check_Horizontal].ItemID_Occupied;
				if (InventoryItemID_Check != InventoryItemID_LeftTop) {
					//Exception there is multiple inventory item id on target room
					return;
				}
			}
		}

		//Free room
		for (int32 index_Free_Vertical = SlotToFree_LeftTop.Y; index_Free_Vertical <= SlotToFree_RightBottom.Y; ++index_Free_Vertical) {
			for (int32 index_Free_Horizontal = SlotToFree_LeftTop.X; index_Free_Horizontal <= SlotToFree_RightBottom.X; ++index_Free_Horizontal) {
				m_InventorySlotData[index_Free_Vertical].SlotDataRow[index_Free_Horizontal].Reset();
			}
		}
	}



	////If ForceFree Set as true, skip all the check process and free all target room which available

	//FIntPoint SlotToFree_LeftTop = FIntPoint(_index_Horizontal, _index_Vertical);
	//FIntPoint SlotToFree_RightBottom = SlotToFree_LeftTop;
	//SlotToFree_RightBottom.X = SlotToFree_LeftTop.X + _size_Horizontal - 1;
	//SlotToFree_RightBottom.Y = SlotToFree_LeftTop.Y + _size_Vertical - 1;

	//if (_bForceFree == false) {
	//	//Valid Check
	//	if (_index_Horizontal < 0 || _index_Vertical < 0) {		
	//		//Note : 이부분은 지워도 무관함. 사이즈가 음수가 나와 우측하단 위치가 좌상단으로 오게될 순 있지만 시작 인덱스 유효검사는 어차피 따로 실행하므로 음수인지 검사를 할 필요는 없음.
	//		// 그냥 이왕 검사하는거 보기좋게 가능한건 다 검사하자고 넣었을 뿐.
	//		//Exception Negative SlotIndex
	//		return;
	//	}

	//	if (_size_Horizontal <= 0 || _size_Vertical <= 0) {
	//		//Exception Unavailable item size
	//		return;
	//	}

	//	bool IsValid_Index_LeftTop = Check_IsValidIndex(SlotToFree_LeftTop.X, SlotToFree_LeftTop.Y);
	//	if (IsValid_Index_LeftTop == false) {
	//		//Exception Invalid Index
	//		return;
	//	}

	//	bool IsValid_Index_RightBottom = Check_IsValidIndex(SlotToFree_RightBottom.X, SlotToFree_RightBottom.Y);
	//	if (IsValid_Index_RightBottom == false) {
	//		//Exception Invalid Index
	//		return;
	//	}
	//}
	//else {
	//	//Resize Target RoomZone if it is on outside of inventory radius
	//	bool IsValid_Index_LeftTop = Check_IsValidIndex(SlotToFree_LeftTop.X, SlotToFree_LeftTop.Y);
	//	if (IsValid_Index_LeftTop == false) {
	//		if (SlotToFree_LeftTop.X < 0) {
	//			SlotToFree_LeftTop.X = 0;
	//		}
	//		else {
	//			
	//		}
	//	}
	//}
}
