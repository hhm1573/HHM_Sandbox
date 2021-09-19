// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile/Base/HHM_Tile.h"

#include "Data/Inventory/Base/HHM_Data_Inventory.h"
#include "Data/Item/ItemData/HHM_ItemData.h"

#include "HHM_Tile_ItemContainer.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class HHM_SANDBOX_API AHHM_Tile_ItemContainer : public AHHM_Tile
{
	GENERATED_BODY()
	
public:
	AHHM_Tile_ItemContainer();



protected:
	UPROPERTY()
		FHHM_Data_Inventory		m_Data_Inventory;

public:
	virtual FHHM_TileData On_Placed(class ALocalMap* _pLocalMap, FHHM_TileData& _tileInfo, class AEntity* _pEntity);
	virtual FHHM_TileData On_Destruct(class ALocalMap* _pLocalMap, FHHM_TileData& _tileInfo);



public:
	FIntPoint	Get_Size_Inventory() const { return FIntPoint(m_Data_Inventory.m_Size_Horizontal, m_Data_Inventory.m_Size_Vertical); }
	FHHM_Data_Inventory		Get_Data_Inventory() const { return m_Data_Inventory; }



public:
	virtual bool On_Item_Add(class ALocalMap* _pLocalMap, FHHM_TileData& _tileData, class AEntity* _pEntity, UHHM_ItemData* _pItemData);
	virtual bool On_Item_Add_At(class ALocalMap* _pLocalMap, FHHM_TileData& _tileData, class AEntity* _pEntity, UHHM_ItemData* _pItemData, int32 _index_Inventory_Horizontal, int32 _index_Inventory_Vertical);
	//Remove 함수와 Delete 함수에 딱히 차이는 없음. 둘 다 이미 외부에서 지우고자 하는 아이템을 갖고있어, Remove_At과 달리 아이템을 받아오거나 하는 과정이 필요없기 때문.
	virtual bool On_Item_Remove(class ALocalMap* _pLocalMap, FHHM_TileData& _tileData, class AEntity* _pEntity, UHHM_ItemData* _pItemData_Remove);
	virtual bool On_Item_Remove_At(class ALocalMap* _pLocalMap, FHHM_TileData& _tileData, class AEntity* _pEntity, UHHM_ItemData*& _pItemData_Return, int32 _index_Inventory_Horizontal, int32 _index_Inventory_Vertical);
	virtual bool On_Item_Delete(class ALocalMap* _pLocalMap, FHHM_TileData& _tileData, class AEntity* _pEntity, UHHM_ItemData* _pItemData_Delete);
	virtual bool On_Item_Delete_At(class ALocalMap* _pLocalMap, FHHM_TileData& _tileData, class AEntity* _pEntity, int32 _index_Inventory_Horizontal, int32 _index_Inventory_Vertical);

public:
	bool Check_InventoryIndex(int32 _index_Inventory_Horizontal, int32 _index_Inventory_Vertical);

};
