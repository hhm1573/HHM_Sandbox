// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_TileEntity_ItemContainer.h"

#include "Data/LocalMap/LocalMap.h"
#include "Tile/Base/HHM_Tile.h"

#include "Base/Component/Inventory/HHM_Component_Inventory.h"

#include "Tile/Base/HHM_Tile_ItemContainer.h"

#include "Item/Base/HHM_Item.h"



AHHM_TileEntity_ItemContainer::AHHM_TileEntity_ItemContainer() {
	
}

void AHHM_TileEntity_ItemContainer::BeginPlay()
{
	Super::BeginPlay();
}

const FHHM_Inventory_Grid& AHHM_TileEntity_ItemContainer::Get_Inventory_Const() const
{
	return m_Inventory;
}

bool AHHM_TileEntity_ItemContainer::Initialize_TileEntity(AHHM_Tile* _pTile, ALocalMap* _pLocalMap, int32 _index_Horizontal, int32 _index_Vertical)
{
	bool IsSucceed_ParentInit = Super::Initialize_TileEntity(_pTile, _pLocalMap, _index_Horizontal, _index_Vertical);

	AHHM_Tile_ItemContainer* pTile_ItemContainer = nullptr;
	pTile_ItemContainer = Cast<AHHM_Tile_ItemContainer>(_pTile);
	if (pTile_ItemContainer == false) {
		//Exception Tile Casting Failed
		return false;
	}

	FHHM_Data_Inventory Data_Inventory = pTile_ItemContainer->Get_Data_Inventory();
	if (Data_Inventory.m_Size_Horizontal <= 0 || Data_Inventory.m_Size_Vertical <= 0) {
		//Exception inventory size too small
		return false;
	}

	bool IsSucceed_Initialize = m_Inventory.Initialize_Inventory(Data_Inventory);
	if (IsSucceed_Initialize == false) {
		//Exception Inventory Initialize failed
		return false;
	}
	


	return true;
}

bool AHHM_TileEntity_ItemContainer::Inventory_Item_Add(AHHM_Entity* _pEntity_Interact, UHHM_ItemData*& _pItemData)
{
	if (_pItemData == nullptr) {
		//Exception input item data is nullptr
		return false;
	}

	FIntPoint Size_Item = _pItemData->Get_ItemSize();
	if (Size_Item.X <= 0 || Size_Item.Y <= 0) {
		//Exception Input item has invalid size
		return false;
	}

	bool IsInventoryHasRoomForItem = m_Inventory.Check_IsItemInsertable(_pItemData);
	if (IsInventoryHasRoomForItem == false) {
		return false;
	}

	int32 Index_ItemAdded = -1;
	EHHM_InventoryReturn InventoryReturn = m_Inventory.Item_Insert(Index_ItemAdded, _pItemData);
	if (Index_ItemAdded < 0) {
		//Exception Insert Item Failed
		return false;
	}
	if (InventoryReturn != EHHM_InventoryReturn::Return_Succeed) {
		//Exception insert item failed
		return false;
	}

	return true;
}

bool AHHM_TileEntity_ItemContainer::Inventory_Item_Add_At(AHHM_Entity* _pEntity_Interact, int32 _index_Inventory_Horizontal, int32 _index_Inventory_Vertical, UHHM_ItemData*& _pItemData)
{
	if (_pItemData == nullptr) {
		//Exception Input item data is nullptr
		return false;
	}

	FIntPoint Size_Item = _pItemData->Get_ItemSize();
	if (Size_Item.X <= 0 || Size_Item.Y <= 0) {
		//Exception Input item has invalid item size
		return false;
	}

	bool IsInventoryHasRoomForItem = m_Inventory.Check_IsItemInsertable_At(_index_Inventory_Horizontal, _index_Inventory_Vertical, _pItemData);
	if (IsInventoryHasRoomForItem == false) {
		return false;
	}

	int32 InventoryItemID = -1;
	EHHM_InventoryReturn InventoryReturn = m_Inventory.Item_Insert_At(InventoryItemID, _pItemData, _index_Inventory_Horizontal, _index_Inventory_Vertical);
	if (InventoryReturn != EHHM_InventoryReturn::Return_Succeed) {
		//Exception Insert item faild
		return false;
	}

	return true;
}

bool AHHM_TileEntity_ItemContainer::Inventory_Item_Remove_At(UHHM_ItemData*& _pItemData_Return, AHHM_Entity* _pEntity_Interact, int32 _index_Inventory_Horizontal, int32 _index_Inventory_Vertical)
{
	if (_pItemData_Return != nullptr) {
		//Exception [Notice] recommend to set return item data as nullptr
	}

	bool IsValid_Index_Inventory = m_Inventory.Check_IsValidIndex(_index_Inventory_Horizontal, _index_Inventory_Vertical);
	if (IsValid_Index_Inventory == false) {
		return false;
	}

	UHHM_ItemData* ItemData_Remove = nullptr;
	EHHM_InventoryReturn InventoryReturn = m_Inventory.Item_Pop_At(ItemData_Remove, _index_Inventory_Horizontal, _index_Inventory_Vertical);
	if (InventoryReturn != EHHM_InventoryReturn::Return_Succeed) {
		//Exception Remove item from inventory failed
		return false;
	}

	_pItemData_Return = ItemData_Remove;
	return true;
}

bool AHHM_TileEntity_ItemContainer::Inventory_Item_Delete(AHHM_Entity* _pEntity_Interact, UHHM_ItemData*& _pItemData_Delete)
{
	if (_pItemData_Delete == nullptr) {
		//Exception Input item data is nullptr
		return false;
	}

	EHHM_InventoryReturn InventoryReturn = m_Inventory.Item_Remove(_pItemData_Delete);
	if (InventoryReturn != EHHM_InventoryReturn::Return_Succeed) {
		return false;
	}

	return true;
}

bool AHHM_TileEntity_ItemContainer::Inventory_Item_Delete_At(AHHM_Entity* _pEntity_Interact, int32 _index_Inventory_Horizontal, int32 _index_Inventory_Vertical)
{
	bool IsValid_Index_Inventory = m_Inventory.Check_IsValidIndex(_index_Inventory_Horizontal, _index_Inventory_Vertical);
	if (IsValid_Index_Inventory == false) {
		//Exception Input index is invalid
		return false;
	}

	UHHM_ItemData* ItemData_Delete = nullptr;
	EHHM_InventoryReturn InventoryReturn = m_Inventory.Get_ItemDataPtr_AtSlot(ItemData_Delete, _index_Inventory_Horizontal, _index_Inventory_Vertical);
	if (InventoryReturn != EHHM_InventoryReturn::Return_Succeed) {
		//Exception No item on target index
		return false;
	}

	EHHM_InventoryReturn InventoryReturn_ItemPop = m_Inventory.Item_Pop_At(ItemData_Delete, _index_Inventory_Horizontal, _index_Inventory_Vertical);
	if (InventoryReturn_ItemPop != EHHM_InventoryReturn::Return_Succeed) {
		//Exception Remove item from inventory failed
		return false;
	}

	return true;
}

FIntPoint AHHM_TileEntity_ItemContainer::Get_InventorySize() const
{
	return m_Inventory.Get_InventorySize();
}

bool AHHM_TileEntity_ItemContainer::Check_InventoryHasRoomForItem(UHHM_ItemData*& _pItemData)
{
	if (_pItemData == nullptr) {
		//Exception Input Item Data is nullptr
		return false;
	}

	FIntPoint Size_Item = _pItemData->Get_ItemSize();
	if (Size_Item.X <= 0 || Size_Item.Y <= 0) {
		//Exception Input item has invalid size data
		return false;
	}

	bool IsInventoryHasRoomForItem = m_Inventory.Check_IsItemInsertable(_pItemData);
	return IsInventoryHasRoomForItem;
}
