// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Component_Inventory.h"

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



bool UHHM_Component_Inventory::Insert_Item_At(int32 _index_Horizontal, int32 _index_Vertical, TSharedPtr<UHHM_Item>& _pItem)
{
	if (_pItem == nullptr) {
		//Exception no item
		return false;
	}

	bool IsValidIndex = Check_IsValidIndex(_index_Horizontal, _index_Vertical);
	if (IsValidIndex == false) {
		//Exception input index was not valid
		return false;
	}

	FIntPoint ItemSize = _pItem->Get_ItemSize();
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

	bool IsKeyOccupied = m_Container_Item.Contains(ValidItemKey);
	if (IsKeyOccupied == true) {
		//Exception Internal structure error if it happens fix Find_ValidItemKey Func
		return false;
	}

	bool IsSucceed_OccupyingRoom = Set_RoomOccupied(_index_Horizontal, _index_Vertical, ItemSize.X, ItemSize.Y, ValidItemKey);
	if (IsSucceed_OccupyingRoom == false) {
		//Exception Occupying Room Failed for some reason
		Set_RoomFree(_index_Horizontal, _index_Vertical, ItemSize.X, ItemSize.Y);
		return false;
	}

	m_Container_Item.Add(ValidItemKey, _pItem);

	return true;
}

int32 UHHM_Component_Inventory::Intert_Item(TSharedPtr<UHHM_Item>& _pItem)
{
	if (_pItem == nullptr) {
		//Exception Input Item Is Nullptr
		return -1;
	}

	FIntPoint Size_Item = _pItem->Get_ItemSize();
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

	bool IsKeyOccupied = m_Container_Item.Contains(ValidItemKey);
	if (IsKeyOccupied == true) {
		//Exception internal structure error
		return -1;
	}

	int32 Index_Horizontal = Index_Found % m_InventorySize_Vertical;
	int32 Index_Vertical = Index_Found / m_InventorySize_Vertical;



	bool IsSucceed_OccupyingRoom = Set_RoomOccupied(Index_Horizontal, Index_Vertical, Size_Item.X, Size_Item.Y, ValidItemKey);
	if (IsSucceed_OccupyingRoom == false) {
		Set_RoomFree(Index_Horizontal, Index_Vertical, Size_Item.X, Size_Item.Y);
		return -1;
	}

	m_Container_Item.Add(ValidItemKey, _pItem);

	return Index_Found;
}



FIntPoint UHHM_Component_Inventory::Convert_IndexToIndexPoint(int32 _index)
{
	int32 Index_Horizontal = _index % m_InventorySize_Vertical;
	int32 Index_Vertical = _index / m_InventorySize_Vertical;

	return FIntPoint(Index_Horizontal, Index_Vertical);
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

bool UHHM_Component_Inventory::Check_IsValidIndex(int32 _index_Horizontal, int32 _index_Vertical)
{
	return _index_Horizontal < 0 || _index_Horizontal >= m_InventorySize_Horizontal
		|| _index_Vertical < 0 || _index_Vertical >= m_InventorySize_Vertical ? false : true;
}

int32 UHHM_Component_Inventory::Find_ValidItemKey()
{
	int32 Index_Searching = 0;
	while (Index_Searching < 5000) {
		bool Is_KeyOccupied = m_Container_Item.Contains(Index_Searching);
		if (Is_KeyOccupied == false) {
			return Index_Searching;
		}
		++Index_Searching;
	}
	//Exception Too many items
	return -1;
}

bool UHHM_Component_Inventory::Set_RoomOccupied(int32 _index_Horizontal, int32 _index_Vertical, int32 _size_Horizontal, int32 _size_Vertical, int32 _itemKey)
{
	bool IsValidKey = m_Container_Item.Contains(_itemKey);							// HHM Note : 변수체크 부분이 중복될 수 있음.
	if (IsValidKey == false) {
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

