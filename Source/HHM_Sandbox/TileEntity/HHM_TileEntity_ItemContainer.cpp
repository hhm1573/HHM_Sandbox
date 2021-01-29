// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_TileEntity_ItemContainer.h"

#include "Data/LocalMap/LocalMap.h"
#include "Tile/Base/HHM_Tile.h"

#include "Base/Component/Inventory/HHM_Component_Inventory.h"

#include "Tile/Base/HHM_Tile_ItemContainer.h"

#include "Item/Base/HHM_Item.h"



AHHM_TileEntity_ItemContainer::AHHM_TileEntity_ItemContainer() {

	if (m_pComponent_Inventory == nullptr) {
		m_pComponent_Inventory = CreateDefaultSubobject<UHHM_Component_Inventory>(TEXT("HHM_InventoryComponent"));
	}
}

void AHHM_TileEntity_ItemContainer::BeginPlay()
{
	Super::BeginPlay();
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

	FIntPoint Size_Inventory = pTile_ItemContainer->Get_Size_Inventory();
	if (Size_Inventory.X <= 0 || Size_Inventory.Y <= 0) {
		//Exception 
	}
	bool IsSucceed_Initialize_Inventory = m_pComponent_Inventory->Initialize_Inventory(Size_Inventory.X, Size_Inventory.Y);
	if (IsSucceed_Initialize_Inventory == false) {
		//Exception
		return false;
	}

	return true;
}

bool AHHM_TileEntity_ItemContainer::Inventory_Item_Add(AEntity* _pEntity_Interact, TSharedPtr<UHHM_ItemData>& _pItemData)
{
	if (m_pComponent_Inventory == nullptr) {
		//Exception [Warning] inventory component not created
		return false;
	}

	if (_pItemData == nullptr) {
		//Exception input item data is nullptr
		return false;
	}

	if (_pItemData->Get_Item() == nullptr) {
		//Exception Input item data has no item pointer
		return false;
	}

	FIntPoint Size_Item = _pItemData->Get_Item()->Get_ItemSize();
	if (Size_Item.X <= 0 || Size_Item.Y <= 0) {
		//Exception Input item has invalid size
		return false;
	}

	bool IsInventoryHasRoomForItem = m_pComponent_Inventory->Check_IsItemInsertable(_pItemData);
	if (IsInventoryHasRoomForItem == false) {
		return false;
	}

	int32 Index_ItemAdded = m_pComponent_Inventory->Item_Insert(_pItemData);
	if (Index_ItemAdded < 0) {
		//Exception Insert Item Failed
		return false;
	}

	return true;
}

bool AHHM_TileEntity_ItemContainer::Inventory_Item_Add_At(AEntity* _pEntity_Interact, int32 _index_Inventory_Horizontal, int32 _index_Inventory_Vertical, TSharedPtr<UHHM_ItemData>& _pItemData)
{
	if (m_pComponent_Inventory == nullptr) {
		//Exception Inventory Component not created
		return false;
	}

	if (_pItemData == nullptr) {
		//Exception Input item data is nullptr
		return false;
	}

	if (_pItemData->Get_Item() == nullptr) {
		//Exception Input item data has no item pointer
		return false;
	}

	FIntPoint Size_Item = _pItemData->Get_Item()->Get_ItemSize();
	if (Size_Item.X <= 0 || Size_Item.Y <= 0) {
		//Exception Input item has invalid item size
		return false;
	}

	bool IsInventoryHasRoomForItem = m_pComponent_Inventory->Check_IsItemInsertableAt(_index_Inventory_Horizontal, _index_Inventory_Vertical, _pItemData);
	if (IsInventoryHasRoomForItem == false) {
		return false;
	}

	bool IsSucceed_InsertItem = m_pComponent_Inventory->Item_Insert_At(_index_Inventory_Horizontal, _index_Inventory_Vertical, _pItemData);
	if (IsSucceed_InsertItem == false) {
		//Exception Insert item faild
		return false;
	}

	return true;
}

bool AHHM_TileEntity_ItemContainer::Inventory_Item_Remove_At(TSharedPtr<UHHM_ItemData>& _pItemData_Return, AEntity* _pEntity_Interact, int32 _index_Inventory_Horizontal, int32 _index_Inventory_Vertical)
{
	if (m_pComponent_Inventory == nullptr) {
		//Exception [Warning] Inventoy Component not created
		return false;
	}

	if (_pItemData_Return != nullptr) {
		//Exception [Notice] recommend to set return item data as nullptr
	}

	bool IsValid_Index_Inventory = m_pComponent_Inventory->Check_IsValidIndex(_index_Inventory_Horizontal, _index_Inventory_Vertical);
	if (IsValid_Index_Inventory == false) {
		return false;
	}

	TSharedPtr<UHHM_ItemData> ItemData_Remove = nullptr;
	bool IsSucceed_RemoveItem = m_pComponent_Inventory->Item_Pop_At(ItemData_Remove, _index_Inventory_Horizontal, _index_Inventory_Vertical);
	if (IsSucceed_RemoveItem == false) {
		//Exception Remove item from inventory failed
		return false;
	}

	_pItemData_Return = ItemData_Remove;
	return true;
}

bool AHHM_TileEntity_ItemContainer::Inventory_Item_Delete(AEntity* _pEntity_Interact, TSharedPtr<UHHM_ItemData>& _pItemData_Delete)
{
	if (m_pComponent_Inventory == nullptr) {
		//Exception Inventory component not created
		return false;
	}

	if (_pItemData_Delete == nullptr) {
		//Exception Input item data is nullptr
		return false;
	}

	bool IsSucceed_RemoveItem = m_pComponent_Inventory->Item_Remove(_pItemData_Delete);
	if (IsSucceed_RemoveItem == false) {
		return false;
	}

	return true;
}

bool AHHM_TileEntity_ItemContainer::Inventory_Item_Delete_At(AEntity* _pEntity_Interact, int32 _index_Inventory_Horizontal, int32 _index_Inventory_Vertical)
{
	if (m_pComponent_Inventory == nullptr) {
		//Exception Inventory component not created
		return false;
	}

	bool IsValid_Index_Inventory = m_pComponent_Inventory->Check_IsValidIndex(_index_Inventory_Horizontal, _index_Inventory_Vertical);
	if (IsValid_Index_Inventory == false) {
		//Exception Input index is invalid
		return false;
	}

	TSharedPtr<UHHM_ItemData> ItemData_Delete = nullptr;
	ItemData_Delete = m_pComponent_Inventory->Get_ItemPtr(_index_Inventory_Horizontal, _index_Inventory_Vertical);
	if (ItemData_Delete == nullptr) {
		//Exception No item on target index
		return false;
	}

	bool IsSucceed_RemoveItem = m_pComponent_Inventory->Item_Pop_At(ItemData_Delete, _index_Inventory_Horizontal, _index_Inventory_Vertical);
	if (IsSucceed_RemoveItem == false) {
		//Exception Remove item from inventory failed
		return false;
	}

	return true;
}

FIntPoint AHHM_TileEntity_ItemContainer::Get_InventorySize() const
{
	return m_pComponent_Inventory == nullptr ? FIntPoint(-1, -1) : m_pComponent_Inventory->Get_InventorySize();
}

bool AHHM_TileEntity_ItemContainer::Check_InventoryHasRoomForItem(TSharedPtr<UHHM_ItemData>& _pItemData)
{
	if (m_pComponent_Inventory == nullptr) {
		//Exception [Warning] Inventory component not created
		return false;
	}

	if (_pItemData == nullptr) {
		//Exception Input Item Data is nullptr
		return false;
	}

	if (_pItemData->Get_Item() == nullptr) {
		//Exceptino Input item has no valid item pointer
		return false;
	}

	FIntPoint Size_Item = _pItemData->Get_Item()->Get_ItemSize();
	if (Size_Item.X <= 0 || Size_Item.Y <= 0) {
		//Exception Input item has invalid size data
		return false;
	}

	bool IsInventoryHasRoomForItem = m_pComponent_Inventory->Check_IsItemInsertable(_pItemData);
	return IsInventoryHasRoomForItem;
}
