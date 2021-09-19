// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Tile_ItemContainer.h"

#include "Engine/World.h"
#include "Manager/Math/Grid/HHM_Manager_Math_Grid.h"

#include "Data/LocalMap/LocalMap.h"
#include "Entity/HHM_Entity.h"

#include "TileEntity/HHM_TileEntity.h"
#include "TileEntity/HHM_TileEntity_ItemContainer.h"



AHHM_Tile_ItemContainer::AHHM_Tile_ItemContainer() {

}



FHHM_TileData AHHM_Tile_ItemContainer::On_Placed(ALocalMap* _pLocalMap, FHHM_TileData& _tileInfo, AEntity* _pEntity)
{
	UWorld* pWorld = nullptr;
	pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception Critical Cant Get World Instance
		return m_pManager_Tile->Get_DefaultTileInfo_ByID(0);
	}

	const FHHM_MapInfo& MapInfo_ConstRef = _pLocalMap->Get_MapInfo_ConstRef();
	FVector Location_Tile = FVector(-1.0f);
	AHHM_Manager_Math_Grid::Convert_IndexLocation_To_Translation(Location_Tile, FVector2D(_tileInfo.Index_Horizontal, _tileInfo.Index_Vertical), MapInfo_ConstRef);
	if (Location_Tile.X < 0 || Location_Tile.Z < 0) {
		//Exception Invalid Location for tile entity. IndexLocation of tile data might not set correctly
		return m_pManager_Tile->Get_DefaultTileInfo_ByID(0);
	}

	AHHM_TileEntity_ItemContainer* pTileEntity_Create = nullptr;
	pTileEntity_Create = pWorld->SpawnActor<AHHM_TileEntity_ItemContainer>(Location_Tile, FRotator::ZeroRotator);
	if (pTileEntity_Create == nullptr) {
		//Exception creating tileentity failed
		return m_pManager_Tile->Get_DefaultTileInfo_ByID(0);
	}
	
	bool IsSucceed_SetTileEntity = _pLocalMap->Set_TileEntity_At(_tileInfo.Index_Horizontal, _tileInfo.Index_Vertical, TSharedPtr<AHHM_TileEntity>(pTileEntity_Create));
	if (IsSucceed_SetTileEntity == false) {
		//Exception Set tile entity on localmap failed
		return m_pManager_Tile->Get_DefaultTileInfo_ByID(0);
	}



	return _tileInfo;
}

FHHM_TileData AHHM_Tile_ItemContainer::On_Destruct(ALocalMap* _pLocalMap, FHHM_TileData& _tileInfo)
{
	//Spill All The Content

	// HHM Note : Call parent's On_Destruct function because tile entity deleting process is on Parent's function
	FHHM_TileData TileData_Air = Super::On_Destruct(_pLocalMap, _tileInfo);
	return TileData_Air;
}



bool AHHM_Tile_ItemContainer::On_Item_Add(ALocalMap* _pLocalMap, FHHM_TileData& _tileData, AEntity* _pEntity, UHHM_ItemData* _pItemData)
{
	if (_pItemData == nullptr) {
		//Exception ItemData is nullptr
		return false;
	}



	if (_pLocalMap == nullptr) {
		//Exception Input localmap is nullptr
		return false;
	}

	TSharedPtr<AHHM_TileEntity> pTileEntity = nullptr;
	bool IsSucceed_Get_TileEntity = _pLocalMap->Get_TileEntity_At(pTileEntity, _tileData.Index_Horizontal, _tileData.Index_Vertical);
	if (IsSucceed_Get_TileEntity == false) {
		//Exception get tile entity failed
		return false;
	}
	if (pTileEntity == nullptr) {
		//Exception Tile entity is nullptr
		return false;
	}

	AHHM_TileEntity_ItemContainer* pTileEntity_ItemContainer = nullptr;
	pTileEntity_ItemContainer = Cast<AHHM_TileEntity_ItemContainer>(pTileEntity.Get());
	if (pTileEntity_ItemContainer == nullptr) {
		//Exception Casting tile entity failed
		return false;
	}



	bool IsSucceed_AddItem = pTileEntity_ItemContainer->Inventory_Item_Add(_pEntity, _pItemData);
	if (IsSucceed_AddItem == false) {
		//Exception Adding item failed
		return false;
	}



	return true;
}

bool AHHM_Tile_ItemContainer::On_Item_Add_At(ALocalMap* _pLocalMap, FHHM_TileData& _tileData, AEntity* _pEntity, UHHM_ItemData* _pItemData, int32 _index_Inventory_Horizontal, int32 _index_Inventory_Vertical)
{
	if (_pItemData == nullptr) {
		//Exception Input item is nullptr
		return false;
	}

	bool IsValid_Index_Inventory = Check_InventoryIndex(_index_Inventory_Horizontal, _index_Inventory_Vertical);
	if (IsValid_Index_Inventory == false) {
		//Exception Invalid Inventory Index
		return false;
	}

	

	if (_pLocalMap == nullptr) {
		//Exception input localmap is nullptr
		return false;
	}

	TSharedPtr<AHHM_TileEntity> pTileEntity_Raw = nullptr;
	bool IsSucceed_GetTileEntity = _pLocalMap->Get_TileEntity_At(pTileEntity_Raw, _tileData.Index_Horizontal, _tileData.Index_Vertical);
	if (IsSucceed_GetTileEntity == false) {
		//Exception Get TileEntity Failed
		return false;
	}
	if (pTileEntity_Raw == nullptr) {
		//Exception TileEntity is nullptr
		return false;
	}

	AHHM_TileEntity_ItemContainer* pTileEntity = nullptr;
	pTileEntity = Cast<AHHM_TileEntity_ItemContainer>(pTileEntity_Raw.Get());
	if (pTileEntity == nullptr) {
		//Exception Cast Tile Entity failed
		return false;
	}
	


	bool IsSucceed_AddItem = pTileEntity->Inventory_Item_Add_At(_pEntity, _index_Inventory_Horizontal, _index_Inventory_Vertical, _pItemData);
	if (IsSucceed_AddItem == false) {
		//Exception Add Item To Inventory Failed
		return false;
	}



	return true;
}

bool AHHM_Tile_ItemContainer::On_Item_Remove(ALocalMap* _pLocalMap, FHHM_TileData& _tileData, AEntity* _pEntity, UHHM_ItemData* _pItemData_Remove)
{
	if (_pItemData_Remove == nullptr) {
		//Exception Input ItemData is nullptr
		return false;
	}

	

	if (_pLocalMap == nullptr) {
		//Exception Input LocalMap is nullptr
		return false;
	}

	TSharedPtr<AHHM_TileEntity> pTileEntity_Raw = nullptr;
	bool IsSucceed_GetTileEntity = _pLocalMap->Get_TileEntity_At(pTileEntity_Raw, _tileData.Index_Horizontal, _tileData.Index_Vertical);
	if (IsSucceed_GetTileEntity == false) {
		//Exception Get Tile Entity failed
		return false;
	}
	if (pTileEntity_Raw == nullptr) {
		//Exception TileEntity is nullptr
		return false;
	}

	AHHM_TileEntity_ItemContainer* pTileEntity = nullptr;
	pTileEntity = Cast<AHHM_TileEntity_ItemContainer>(pTileEntity_Raw.Get());
	if (pTileEntity == nullptr) {
		//Exception Cast TileEntity Failed
		return false;
	}



	bool IsSucceed_RemoveItem = pTileEntity->Inventory_Item_Delete(_pEntity, _pItemData_Remove);
	if (IsSucceed_RemoveItem == false) {
		//Exception Remove Item From Inventory failed
		return false;
	}



	return true;
}

bool AHHM_Tile_ItemContainer::On_Item_Remove_At(ALocalMap* _pLocalMap, FHHM_TileData& _tileData, AEntity* _pEntity, UHHM_ItemData*& _pItemData_Return, int32 _index_Inventory_Horizontal, int32 _index_Inventory_Vertical)
{
	bool IsValidIndex_Inventory = Check_InventoryIndex(_index_Inventory_Horizontal, _index_Inventory_Vertical);
	if (IsValidIndex_Inventory == false) {
		//Exception Input Inventory Index is invalid
		return false;
	}



	if (_pLocalMap == nullptr) {
		//Exception Input LocalMap is nullptr
		return false;
	}

	TSharedPtr<AHHM_TileEntity> pTileEntity_Raw = nullptr;
	bool IsSucceed_GetTileEntity = _pLocalMap->Get_TileEntity_At(pTileEntity_Raw, _tileData.Index_Horizontal, _tileData.Index_Vertical);
	if (IsSucceed_GetTileEntity == false) {
		//Exception Get Tile Entity Failed
		return false;
	}
	if (pTileEntity_Raw == nullptr) {
		//Exception TileEntity is nullptr
		return false;
	}

	AHHM_TileEntity_ItemContainer* pTileEntity = nullptr;
	pTileEntity = Cast<AHHM_TileEntity_ItemContainer>(pTileEntity_Raw.Get());
	if (pTileEntity == nullptr) {
		//Exception Cast tile entity failed
		return false;
	}



	UHHM_ItemData* pItemData_Remove = nullptr;
	bool IsSucceed_ItemRemove = pTileEntity->Inventory_Item_Remove_At(pItemData_Remove, _pEntity, _index_Inventory_Horizontal, _index_Inventory_Vertical);
	if (IsSucceed_ItemRemove == false) {
		//Exception Remove item from inventory failed (pop item)
		return false;
	}
	if (pItemData_Remove == nullptr) {
		//Exception Removed item data is nullptr
		return false;
	}

	_pItemData_Return = pItemData_Remove;
	return true;
}

bool AHHM_Tile_ItemContainer::On_Item_Delete(ALocalMap* _pLocalMap, FHHM_TileData& _tileData, AEntity* _pEntity, UHHM_ItemData* _pItemData_Delete)
{
	if (_pItemData_Delete == nullptr) {
		//Exception Input Item Data is nullptr
		return false;
	}



	if (_pLocalMap == nullptr) {
		//Excpetion Input Localmap is nullptr
		return false;
	}

	TSharedPtr<AHHM_TileEntity> pTileEntity_Raw = nullptr;
	bool IsSucceed_GetTileEntity = _pLocalMap->Get_TileEntity_At(pTileEntity_Raw, _tileData.Index_Horizontal, _tileData.Index_Vertical);
	if (IsSucceed_GetTileEntity == false) {
		//Exception Get Tile entity failed
		return false;
	}
	if (pTileEntity_Raw == nullptr) {
		//Exception tile entity is nullptr
		return false;
	}

	AHHM_TileEntity_ItemContainer* pTileEntity = nullptr;
	pTileEntity = Cast<AHHM_TileEntity_ItemContainer>(pTileEntity_Raw.Get());
	if (pTileEntity == nullptr) {
		//Exception cast tile entity failed
		return false;
	}

	

	bool IsSucceed_DeleteItem = pTileEntity->Inventory_Item_Delete(_pEntity, _pItemData_Delete);
	if (IsSucceed_DeleteItem == false) {
		//Exception Delete item from inventory failed
		return false;
	}



	return true;
}

bool AHHM_Tile_ItemContainer::On_Item_Delete_At(ALocalMap* _pLocalMap, FHHM_TileData& _tileData, AEntity* _pEntity, int32 _index_Inventory_Horizontal, int32 _index_Inventory_Vertical)
{
	bool IsValidIndex_Inventory = Check_InventoryIndex(_index_Inventory_Horizontal, _index_Inventory_Vertical);
	if (IsValidIndex_Inventory == false) {
		//Exception input inventory index is invalid
		return false;
	}



	if (_pLocalMap == nullptr) {
		//Exception Input local map is nullptr
		return false;
	}

	TSharedPtr<AHHM_TileEntity> pTileEntity_Raw = nullptr;
	bool IsSucceed_GetTileEntity = _pLocalMap->Get_TileEntity_At(pTileEntity_Raw, _tileData.Index_Horizontal, _tileData.Index_Vertical);
	if (IsSucceed_GetTileEntity == false) {
		//Exception Get Tile Entity failed
		return false;
	}
	if (pTileEntity_Raw == nullptr) {
		//Exception tile entity is nullptr
		return false;
	}

	AHHM_TileEntity_ItemContainer* pTileEntity = nullptr;
	pTileEntity = Cast<AHHM_TileEntity_ItemContainer>(pTileEntity_Raw.Get());
	if (pTileEntity == nullptr) {
		//Exception cast tile entity failed
		return false;
	}



	bool IsSucceed_DeleteItem = pTileEntity->Inventory_Item_Delete_At(_pEntity, _index_Inventory_Horizontal, _index_Inventory_Vertical);
	if (IsSucceed_DeleteItem == false) {
		//Exception Delete item from inventory failed
		return false;
	}



	return true;
}

bool AHHM_Tile_ItemContainer::Check_InventoryIndex(int32 _index_Inventory_Horizontal, int32 _index_Inventory_Vertical)
{
	if (_index_Inventory_Horizontal < 0 || _index_Inventory_Horizontal >= m_Data_Inventory.m_Size_Horizontal
		|| _index_Inventory_Vertical < 0 || _index_Inventory_Vertical >= m_Data_Inventory.m_Size_Vertical) {
		return false;
	}

	return true;
}
