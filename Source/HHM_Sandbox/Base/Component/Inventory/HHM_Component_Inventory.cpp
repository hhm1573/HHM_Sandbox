// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Component_Inventory.h"

#include "Item/Base/HHM_Item.h"



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



bool UHHM_Component_Inventory::Initialize_Inventory(int32 _size_Horizontal, int32 _size_Vertical)
{
	if (_size_Horizontal <= 0 || _size_Vertical <= 0) {
		return false;
	}

	int32 Num_InventoryRow = m_InventorySlotData.Num();
	for (int32 index_Row = 0; index_Row < Num_InventoryRow; ++index_Row) {
		m_InventorySlotData[index_Row].SlotDataRow.Empty();
	}
	m_InventorySlotData.Empty();

	

	FHHM_InventorySlotData SlotData_Empty = FHHM_InventorySlotData();
	SlotData_Empty.Reset();
	
	m_InventorySlotData.Init(FHHM_InventorySlotData_Row(), _size_Vertical);		//TArray 가 확실히 내 생각처럼 작동하지않을 가능성도 있을수있다 생각하여 윗줄저럼 복사생성하지 않고 그냥 기초생성자를 사용함.
	for (int32 index_Row = 0; index_Row < _size_Vertical; ++index_Row) {
		m_InventorySlotData[index_Row].SlotDataRow.Empty();
		m_InventorySlotData[index_Row].SlotDataRow.Init(SlotData_Empty, _size_Horizontal);
	}

	int32 Num_InventoryRow_After = m_InventorySlotData.Num();
	if (Num_InventoryRow_After != _size_Vertical) {
		//Exception somehow array not fully initialized
		return false;
	}

	int32 Num_InventorySlot_After = m_InventorySlotData[0].SlotDataRow.Num();
	if (Num_InventorySlot_After != _size_Horizontal) {
		//Exception array not initialized as intended
		return false;
	}

	return true;
}



UHHM_ItemData* UHHM_Component_Inventory::Get_ItemDataPtr(int32 _index_Horizontal, int32 _index_Vertical)
{
	bool IsValidIndex = Check_IsValidIndex(_index_Horizontal, _index_Vertical);
	if (IsValidIndex == false) {
		//Exception Invalid Index
		return nullptr;
	}

	bool IsOccupied = m_InventorySlotData[_index_Vertical].SlotDataRow[_index_Horizontal].IsOccupied;
	if (IsOccupied == false) {
		return nullptr;
	}

	int32 Index_Item = m_InventorySlotData[_index_Vertical].SlotDataRow[_index_Horizontal].ItemID_Occupied;
	bool IsItemExisting = m_Container_ItemData.Contains(Index_Item);
	if (IsItemExisting == false) {
		//Exception Inventory slot set as occupied but item key is invalid
		return nullptr;
	}

	return m_Container_ItemData[Index_Item].pItemData;
}



bool UHHM_Component_Inventory::Check_IsValidIndex(int32 _index_Horizontal, int32 _index_Vertical)
{
	return _index_Horizontal < 0 || _index_Horizontal >= m_InventorySize_Horizontal
		|| _index_Vertical < 0 || _index_Vertical >= m_InventorySize_Vertical ? false : true;
}

bool UHHM_Component_Inventory::Check_IsItemSwappable(int32 _index_Horizontal, int32 _index_Vertical, UHHM_ItemData*& _pItemData_Swap)
{
	if (_pItemData_Swap == nullptr) {
		//Exception Input ItemData Is Nullptr
		return false;
	}

	bool IsValidIndex = Check_IsValidIndex(_index_Horizontal, _index_Vertical);
	if (IsValidIndex == false) {
		return false;
	}

	bool IsSlotOccupied = m_InventorySlotData[_index_Vertical].SlotDataRow[_index_Horizontal].IsOccupied;
	if (IsSlotOccupied == false) {
		return false;
	}

	int32 ItemKey_TargetSlot = m_InventorySlotData[_index_Vertical].SlotDataRow[_index_Horizontal].ItemID_Occupied;
	bool IsContainItem = m_Container_ItemData.Contains(ItemKey_TargetSlot);
	if (IsContainItem == false) {
		//Exception ItemData Missing
		return false;
	}

	if (m_Container_ItemData[ItemKey_TargetSlot].pItemData == nullptr) {
		//Exception NULL ItemData Inserted Warning
		return false;
	}

	int32 Num_Slot_ItemOccupying = m_Container_ItemData[ItemKey_TargetSlot].Container_Index_Occupied.Num();
	if (Num_Slot_ItemOccupying <= 0) {
		//Exception No Occupying Index Data At InventoryItemData
		return false;
	}



	//Get Lowest Index item currently occupying
	int32 Index_Occupying_Lowest = m_Container_ItemData[ItemKey_TargetSlot].Container_Index_Occupied[0];
	for (int32 index = 0; index < Num_Slot_ItemOccupying; ++index) {
		int32 Index_Compare = m_Container_ItemData[ItemKey_TargetSlot].Container_Index_Occupied[index];
		if (Index_Occupying_Lowest > Index_Compare) {
			Index_Occupying_Lowest = Index_Compare;
		}
	}

	FIntPoint IndexPoint_Occupying_LeftTop = Convert_IndexToIndexPoint(Index_Occupying_Lowest);
	bool IsValid_Index_LeftTop = Check_IsValidIndex(IndexPoint_Occupying_LeftTop.X, IndexPoint_Occupying_LeftTop.Y);
	if (IsValid_Index_LeftTop == false) {
		//Exception Invalid Index has been saved on InventoryItemData
		return false;
	}

	

	//Check Is Item Fit in remaining space
	FIntPoint ItemSize = m_Container_ItemData[ItemKey_TargetSlot].pItemData->Get_Item()->Get_ItemSize();
	FIntPoint IndexPoint_Occupying_RightBottom = FIntPoint(IndexPoint_Occupying_LeftTop.X + ItemSize.X - 1, IndexPoint_Occupying_LeftTop.Y + ItemSize.Y - 1);
	FIntPoint ItemSize_Source = _pItemData_Swap->Get_Item()->Get_ItemSize();
	FIntPoint IndexPoint_Source_RightBottom = FIntPoint(_index_Horizontal + ItemSize_Source.X - 1, _index_Vertical + ItemSize_Source.Y - 1);

	if (IndexPoint_Source_RightBottom.X > IndexPoint_Occupying_RightBottom.X || IndexPoint_Source_RightBottom.Y > IndexPoint_Occupying_RightBottom.Y) {
		return false;
	}

	return true;
}

bool UHHM_Component_Inventory::Check_IsItemInsertable(UHHM_ItemData*& _pItemData)
{
	if (_pItemData == nullptr) {
		//Exception input item is nullptr
		return false;
	}

	if (_pItemData->Get_Item() == nullptr) {
		//Exception Input item has no item pointer
		return false;
	}

	FIntPoint Size_Item = _pItemData->Get_Item()->Get_ItemSize();
	if (Size_Item.X <= 0 || Size_Item.Y <= 0) {
		//Exception Input item's size is invalid
		return false;
	}

	int32 Index_FreeRoom = Find_FreeRoom(Size_Item.X, Size_Item.Y);
	if (Index_FreeRoom <= 0) {
		return false;
	}

	return true;
}

bool UHHM_Component_Inventory::Check_IsItemInsertableAt(int32 _index_Horizontal, int32 _index_Vertical, UHHM_ItemData*& _pItemData_Insert)
{
	if (_pItemData_Insert == nullptr) {
		return false;
	}

	bool IsValidIndex = Check_IsValidIndex(_index_Horizontal, _index_Vertical);
	if (IsValidIndex == false) {
		//Exception invalid index
		return false;
	}

	FIntPoint Size_Item = _pItemData_Insert->Get_Item()->Get_ItemSize();

	bool IsRoomFree = Check_IsRoomFree(_index_Horizontal, _index_Vertical, Size_Item.X, Size_Item.Y);
	if (IsRoomFree == false) {
		return false;
	}

	return true;
}



bool UHHM_Component_Inventory::Item_Insert_At(int32 _index_Horizontal, int32 _index_Vertical, UHHM_ItemData*& _pItemData)
{
	if (_pItemData == nullptr) {
		//Exception no item
		return false;
	}

	if (_pItemData->Get_Item() == nullptr) {
		//Exception Invalid Item Data
		return false;
	}

	bool IsValidIndex = Check_IsValidIndex(_index_Horizontal, _index_Vertical);
	if (IsValidIndex == false) {
		//Exception input index was not valid
		return false;
	}

	FIntPoint ItemSize = _pItemData->Get_Item()->Get_ItemSize();
	if (ItemSize.X <= 0 || ItemSize.Y <= 0) {
		//Exception Item Size Is not valid
		return false;
	}

	

	bool IsRoomFree = Check_IsRoomFree(_index_Horizontal, _index_Vertical, ItemSize.X, ItemSize.Y);
	if (IsRoomFree == false) {
		return false;
	}

	int32 ValidItemKey = Find_ValidItemKey();
	if (ValidItemKey < 0) {
		//Exception too many item
		return false;
	}

	bool IsKeyOccupied = m_Container_ItemData.Contains(ValidItemKey);
	if (IsKeyOccupied == true) {
		//Exception Internal structure error if it happens fix Find_ValidItemKey Func
		return false;
	}

	FHHM_InventoryItemData InventoryItemData = FHHM_InventoryItemData(_pItemData);

	bool IsSucceed_OccupyingRoom = Set_RoomOccupied(_index_Horizontal, _index_Vertical, ItemSize.X, ItemSize.Y, ValidItemKey, &InventoryItemData);
	if (IsSucceed_OccupyingRoom == false) {
		//Exception Occupying Room Failed for some reason
		Set_RoomFree(_index_Horizontal, _index_Vertical, ItemSize.X, ItemSize.Y);
		return false;
	}

	m_Container_ItemData.Add(ValidItemKey, InventoryItemData);

	return true;
}

int32 UHHM_Component_Inventory::Item_Insert(UHHM_ItemData*& _pItemData)
{
	if (_pItemData == nullptr) {
		//Exception Input Item Is Nullptr
		return -1;
	}

	if (_pItemData->Get_Item() == nullptr) {
		//Exception Invalid Item Data
		return -1;
	}

	FIntPoint Size_Item = _pItemData->Get_Item()->Get_ItemSize();
	if (Size_Item.X <= 0 || Size_Item.Y <= 0) {
		//Exception Item Size Error
		return -1;
	}



	int32 Index_Found = Find_FreeRoom(Size_Item.X, Size_Item.Y);
	if (Index_Found < 0) {
		//Exception Inventory Full
		return -2;
	}

	int32 ValidItemKey = Find_ValidItemKey();
	if (ValidItemKey < 0) {
		//Exception too many item
		return -1;
	}

	bool IsKeyOccupied = m_Container_ItemData.Contains(ValidItemKey);
	if (IsKeyOccupied == true) {
		//Exception internal structure error
		return -1;
	}

	int32 Index_Horizontal = Index_Found % m_InventorySize_Vertical;
	int32 Index_Vertical = Index_Found / m_InventorySize_Vertical;



	FHHM_InventoryItemData InventoryItemData(_pItemData);

	bool IsSucceed_OccupyingRoom = Set_RoomOccupied(Index_Horizontal, Index_Vertical, Size_Item.X, Size_Item.Y, ValidItemKey, &InventoryItemData);
	if (IsSucceed_OccupyingRoom == false) {
		Set_RoomFree(Index_Horizontal, Index_Vertical, Size_Item.X, Size_Item.Y);
		return -1;
	}

	m_Container_ItemData.Add(ValidItemKey, InventoryItemData);

	return Index_Found;
}



bool UHHM_Component_Inventory::Item_Pop_At(UHHM_ItemData*& _pItemData_Return, int32 _index_Horizontal, int32 _index_Vertical)
{
	bool IsValidIndex = Check_IsValidIndex(_index_Horizontal, _index_Vertical);
	if (IsValidIndex == false) {
		//Exception invalid Index
		return false;
	}

	bool IsOccupied = m_InventorySlotData[_index_Vertical].SlotDataRow[_index_Horizontal].IsOccupied;
	if (IsOccupied == false) {
		return false;
	}

	int32 Index_Item = m_InventorySlotData[_index_Vertical].SlotDataRow[_index_Horizontal].ItemID_Occupied;
	bool IsItemExist = m_Container_ItemData.Contains(Index_Item);
	if (IsItemExist == false) {
		//Exception [Warning] 슬롯을 점유하고 있는 아이템이 존재하지 않습니다. 아이템을 삭제하는 과정에서 공간점유를 해제하는 부분에 문제가 생겼을 가능성이 높습니다.
		return false;
	}

	int32 Num_Occupied = m_Container_ItemData[Index_Item].Container_Index_Occupied.Num();
	if (Num_Occupied <= 0) {
		//Exception [Warning] 어떠한 슬롯도 차지하고있지 않는 아이템이 존재합니다. 찌꺼기 아이템일 확률이 높습니다.
		return false;
	}



	for (int32 index = 0; index < Num_Occupied; ++index) {
		FIntPoint IndexPoint_TobeFree = Convert_IndexToIndexPoint(m_Container_ItemData[Index_Item].Container_Index_Occupied[index]);
		bool IsValid_Index_ToBeFree = Check_IsValidIndex(IndexPoint_TobeFree.X, IndexPoint_TobeFree.Y);
		if (IsValid_Index_ToBeFree == false) {
			//Exception [Warning] Item occupied invalid index
			continue;
		}
		m_InventorySlotData[IndexPoint_TobeFree.Y].SlotDataRow[IndexPoint_TobeFree.X].Reset();
	}



	_pItemData_Return = m_Container_ItemData[Index_Item].pItemData;
	m_Container_ItemData.Remove(Index_Item);

	

	return true;
}

bool UHHM_Component_Inventory::Item_Remove(UHHM_ItemData*& _pItemData_Remove)
{
	if (_pItemData_Remove == nullptr) {
		//Exception input pointer is nullptr
		return false;
	}



	int32 Index_Remove = -1;
	for (auto& pair : m_Container_ItemData) {
		if (_pItemData_Remove == pair.Value.pItemData) {
			Index_Remove = pair.Key;
			break;
		}
	}

	if (Index_Remove < 0) {
		return false;
	}



	//Begin CleanUp Process
	int32 Num_ItemOccupying = m_Container_ItemData[Index_Remove].Container_Index_Occupied.Num();
	if (Num_ItemOccupying <= 0) {
		//Exception [Warning] Item that not occupying any slot is on container. this item may a garbage item.
		return false;
	}

	for (int32 index = 0; index < Num_ItemOccupying; ++index) {
		FIntPoint IndexPoint_ToBeFree = Convert_IndexToIndexPoint(m_Container_ItemData[Index_Remove].Container_Index_Occupied[index]);
		bool IsValid_Index_ToBeFree = Check_IsValidIndex(IndexPoint_ToBeFree.X, IndexPoint_ToBeFree.Y);
		if (IsValid_Index_ToBeFree == false) {
			//Exception [Warning] Item Occupying Invalid Slot Index.
			continue;
		}
		m_InventorySlotData[IndexPoint_ToBeFree.Y].SlotDataRow[IndexPoint_ToBeFree.X].Reset();
	}



	m_Container_ItemData.Remove(Index_Remove);

	

	return true;
}

FIntPoint UHHM_Component_Inventory::Convert_IndexToIndexPoint(int32 _index)
{
	int32 Index_Horizontal = _index % m_InventorySize_Vertical;
	int32 Index_Vertical = _index / m_InventorySize_Vertical;

	return FIntPoint(Index_Horizontal, Index_Vertical);
}

int32 UHHM_Component_Inventory::Convert_IndexPointToIndex(FIntPoint _indexPoint)
{
	return _indexPoint.X + _indexPoint.Y * m_InventorySize_Horizontal;
}



bool UHHM_Component_Inventory::Check_IsRoomFree(int32 _index_Horizontal, int32 _index_Vertical, int32 _size_Horizontal, int32 _size_Vertical)
{
	int32 Index_Horizontal_Max = _index_Horizontal + _size_Horizontal - 1;
	int32 Index_Vertical_Max = _index_Vertical + _size_Vertical - 1;

	bool IsValid_Index = Check_IsValidIndex(_index_Horizontal, _index_Vertical);
	bool IsValid_Index_Max = Check_IsValidIndex(Index_Horizontal_Max, Index_Vertical_Max);
	if (IsValid_Index == false || IsValid_Index_Max == false) {
		return false;
	}

	for (int32 index_Row = _index_Vertical; index_Row <= Index_Vertical_Max; ++index_Row) {
		for (int32 index_Slot = _index_Horizontal; index_Slot <= Index_Horizontal_Max; ++index_Slot) {
			if (m_InventorySlotData[index_Row].SlotDataRow[index_Slot].IsOccupied == true) {
				return false;
			}
		}
	}

	return true;
}

int32 UHHM_Component_Inventory::Find_FreeRoom(int32 _size_Horizontal, int32 _size_Vertical)
{
	for (int32 index_Search_Row = 0; index_Search_Row < m_InventorySize_Vertical; ++index_Search_Row) {
		for (int32 index_Search_Slot = 0; index_Search_Slot < m_InventorySize_Horizontal; ++index_Search_Slot) {
			
			if (m_InventorySlotData[index_Search_Row].SlotDataRow[index_Search_Slot].IsOccupied == false) {
				bool IsRoomFree = Check_IsRoomFree(index_Search_Row, index_Search_Slot, _size_Horizontal, _size_Vertical);
				if (IsRoomFree == true) {
					int32 Index_Return = index_Search_Slot + (index_Search_Row * m_InventorySize_Horizontal);
					return Index_Return;
				}
			}

		}
	}

	return -1;
}



int32 UHHM_Component_Inventory::Find_ValidItemKey()
{
	int32 Index_Searching = 0;
	while (Index_Searching < 5000) {
		bool Is_KeyOccupied = m_Container_ItemData.Contains(Index_Searching);
		if (Is_KeyOccupied == false) {
			return Index_Searching;
		}
		++Index_Searching;
	}
	//Exception Too many items
	return -1;
}



bool UHHM_Component_Inventory::Find_FirstSlot_ItemOccupied(FIntPoint& _indexPoint_Return, int32 _index_Item)
{
	bool IsItemExist = m_Container_ItemData.Contains(_index_Item);
	if (IsItemExist == false) {
		//Exception Invalid Item Key
		return false;
	}

	int32 Num_ItemOccupied = m_Container_ItemData[_index_Item].Container_Index_Occupied.Num();
	if (Num_ItemOccupied <= 0) {
		//Exception Warning Item did not occupied any slot. how it even added on inventory? 아무슬롯에도 할당되지않은 찌꺼기 아이템일 확률이 높다.
		return false;
	}

	int32 Index_SlotOccupied_Lowest = m_Container_ItemData[_index_Item].Container_Index_Occupied[0];
	for (int32 index = 1; index < Num_ItemOccupied; ++index) {
		int32 Index_SlotOccupied_Compare = m_Container_ItemData[_index_Item].Container_Index_Occupied[index];
		if (Index_SlotOccupied_Lowest > Index_SlotOccupied_Compare) {
			Index_SlotOccupied_Lowest = Index_SlotOccupied_Compare;
		}
	}

	FIntPoint IndexPoint_Return = Convert_IndexToIndexPoint(Index_SlotOccupied_Lowest);
	_indexPoint_Return = IndexPoint_Return;

	return true;
}



bool UHHM_Component_Inventory::Set_RoomOccupied(int32 _index_Horizontal, int32 _index_Vertical, int32 _size_Horizontal, int32 _size_Vertical, int32 _itemKey, FHHM_InventoryItemData* _inventoryItemData)
{
	bool IsValidKey = m_Container_ItemData.Contains(_itemKey);							// HHM Note : 변수체크 부분이 중복될 수 있음.
	if (IsValidKey == false) {
		return false;
	}

	if (_inventoryItemData == nullptr) {
		return false;
	}

	int32 _index_Horizontal_Max = _index_Horizontal + _size_Horizontal - 1;
	int32 _index_Vertical_Max = _index_Vertical + _size_Vertical - 1;

	bool IsValid_Index = Check_IsValidIndex(_index_Horizontal, _index_Vertical);
	bool IsValid_IndexMax = Check_IsValidIndex(_index_Horizontal_Max, _index_Vertical_Max);
	if (IsValid_Index == false || IsValid_IndexMax == false) {
		return false;
	}

	for (int32 index_Row = _index_Vertical; index_Row <= _index_Vertical_Max; ++_index_Vertical) {
		for (int32 index_Slot = _index_Horizontal; index_Slot <= _index_Horizontal_Max; ++_index_Horizontal) {
			m_InventorySlotData[index_Row].SlotDataRow[index_Slot].Set_Occupied(_itemKey);
			int32 Index_Occupied = Convert_IndexPointToIndex(FIntPoint(index_Slot, index_Row));
			_inventoryItemData->Container_Index_Occupied.Add(Index_Occupied);
		}
	}

	return true;
}

void UHHM_Component_Inventory::Set_RoomFree(int32 _index_Horizontal, int32 _index_Vertical, int32 _size_Horizontal, int32 _size_Vertical, bool _bForceFree)
{
	int32 Index_Horizontal_Max = _index_Horizontal + _size_Horizontal - 1;
	int32 Index_Vertical_Max = _index_Vertical + _size_Vertical - 1;

	bool IsValid_Index = Check_IsValidIndex(_index_Horizontal, _index_Vertical);
	bool IsValid_Index_Max = Check_IsValidIndex(Index_Horizontal_Max, Index_Vertical_Max);

	if (IsValid_Index == false) {
		//Exception Input error
		return;
	}

	if (IsValid_Index_Max == false) {
		if (_bForceFree == true) {
			Index_Horizontal_Max = m_InventorySize_Horizontal - 1;
			Index_Vertical_Max = m_InventorySize_Vertical - 1;
		}
		else {
			//Exception input error
			return;
		}
	}


	
	if (_bForceFree == false) {
		int32 ItemKey_FirstSlot = m_InventorySlotData[_index_Vertical].SlotDataRow[_index_Horizontal].ItemID_Occupied;
		for (int32 index_Row = _index_Vertical; index_Row <= Index_Vertical_Max; ++index_Row) {
			for (int32 index_Slot = _index_Horizontal; index_Slot <= Index_Horizontal_Max; ++index_Slot) {
				int32 ItemKey_CurrentSlot = m_InventorySlotData[index_Row].SlotDataRow[index_Slot].ItemID_Occupied;
				if (ItemKey_FirstSlot != ItemKey_CurrentSlot) {
					//Exception Size error
					return;
				}
			}
		}
	}

	for (int32 index_Row = _index_Vertical; index_Row <= Index_Vertical_Max; ++index_Row) {
		for (int32 index_Slot = _index_Horizontal; index_Slot <= Index_Horizontal_Max; ++index_Slot) {
			m_InventorySlotData[index_Row].SlotDataRow[index_Slot].Reset();
		}
	}

	return;
}

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

