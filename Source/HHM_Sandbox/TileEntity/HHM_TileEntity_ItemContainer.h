// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileEntity/HHM_TileEntity.h"

#include "Data/Inventory/HHM_Inventory.h"
#include "Data/Item/ItemData/HHM_ItemData.h"

#include "HHM_TileEntity_ItemContainer.generated.h"

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API AHHM_TileEntity_ItemContainer : public AHHM_TileEntity
{
	GENERATED_BODY()
	//Add Inventory Component

public:
	AHHM_TileEntity_ItemContainer();

protected:
	virtual void BeginPlay() override;



protected:
	//class UHHM_Component_Inventory* m_pComponent_Inventory = nullptr;
	FHHM_Inventory		m_Inventory;

public:
	virtual bool Initialize_TileEntity(class AHHM_Tile* _pTile, class ALocalMap* _pLocalMap, int32 _index_Horizontal, int32 _index_Vertical) override;



public:
	virtual bool Inventory_Item_Add(class AHHM_Entity* _pEntity_Interact, UHHM_ItemData*& _pItemData);
	virtual bool Inventory_Item_Add_At(class AHHM_Entity* _pEntity_Interact, int32 _index_Inventory_Horizontal, int32 _index_Inventory_Vertical, UHHM_ItemData*& _pItemData);
	virtual bool Inventory_Item_Remove_At(UHHM_ItemData*& _pItemData_Return, class AHHM_Entity* _pEntity_Interact, int32 _index_Inventory_Horizontal, int32 _index_Inventory_Vertical);
	virtual bool Inventory_Item_Delete(class AHHM_Entity* _pEntity_Interact, UHHM_ItemData*& _pItemData_Delete);
	virtual bool Inventory_Item_Delete_At(class AHHM_Entity* _pEntity_Interact, int32 _index_Inventory_Horizontal, int32 _index_Inventory_Vertical);

	FIntPoint	Get_InventorySize() const;

	bool		Check_InventoryHasRoomForItem(UHHM_ItemData*& _pItemData);
};
