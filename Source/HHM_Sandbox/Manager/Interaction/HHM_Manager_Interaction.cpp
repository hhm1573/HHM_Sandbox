// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Manager_Interaction.h"

#include "Entity/HHM_Entity.h"
#include "Data/LocalMap/LocalMap.h"
#include "Header/Struct_LocalMap.h"
#include "Tile/Base/HHM_Tile_Interactable.h"

#include "TileEntity/HHM_TileEntity_ItemContainer.h"
#include "Data/Inventory/HHM_Inventory_Grid.h"
#include "Base/Component/Inventory/HHM_Component_Inventory.h"



AHHM_Manager_Interaction::AHHM_Manager_Interaction() {
	m_Container_InteractionBaseData.Empty();
}



void AHHM_Manager_Interaction::BeginPlay()
{
	Super::BeginPlay();

	Register_AllInteractionBaseData();

	return;
}

void AHHM_Manager_Interaction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	return;
}



bool AHHM_Manager_Interaction::Get_BaseData_Interaction(const FHHM_Data_Interaction_Base*& _pBaseData_Interaction_Return, int32 _id_Interaction)
{
	if (_id_Interaction < 0) {
		//Exception negative id value
		return false;
	}

	FHHM_Data_Interaction_Base* pBaseData_Found = nullptr;
	pBaseData_Found = m_Container_InteractionBaseData.Find(_id_Interaction);

	if (pBaseData_Found == nullptr) {
		//Exception Invalid interaction id
		return false;
	}

	_pBaseData_Interaction_Return = pBaseData_Found;
	return true;
}



void AHHM_Manager_Interaction::Register_AllInteractionBaseData()
{
	//Debug Interact to tile
	m_Container_InteractionBaseData.Add(HHM_INTERACTION_ID_DEBUG_TO_TILE, FHHM_Data_Interaction_Base());
	/*m_Container_InteractionBaseData[HHM_INTERACTION_ID_DEBUG_TO_TILE].m_ID = HHM_INTERACTION_ID_DEBUG_TO_TILE;
	m_Container_InteractionBaseData[HHM_INTERACTION_ID_DEBUG_TO_TILE].m_Name = HHM_INTERACTION_NAME_DEBUG_TO_TILE;
	m_Container_InteractionBaseData[HHM_INTERACTION_ID_DEBUG_TO_TILE].m_Type = EHHM_InteractionType::Type_Interaction_SelfToTile;*/
	m_Container_InteractionBaseData[HHM_INTERACTION_ID_DEBUG_TO_TILE].Set_BaseData(HHM_INTERACTION_ID_DEBUG_TO_TILE, HHM_INTERACTION_NAME_DEBUG_TO_TILE
		, EHHM_InteractionType::Type_Interaction_SelfToTile, 5.0f);

	m_Container_InteractionBaseData.Add(HHM_INTERACTION_ID_DEBUG_GET_ITEM_FROM_TILE, FHHM_Data_Interaction_Base());
	m_Container_InteractionBaseData[HHM_INTERACTION_ID_DEBUG_GET_ITEM_FROM_TILE].Set_BaseData(HHM_INTERACTION_ID_DEBUG_GET_ITEM_FROM_TILE, HHM_INTERACTION_NAME_DEBUG_GET_ITEM_FROM_TILE
		, EHHM_InteractionType::Type_Debug_Item_Get_From_Tile, 1.0f);
}

void AHHM_Manager_Interaction::Execute_Interact(FHHM_Data_Interaction& _data_Interaction, UHHM_Component_InteractionHandler* _pInteractionHandler_Executer)
{
	EHHM_InteractionType InteractionType = _data_Interaction.m_BaseData_Interaction.m_Type;

	switch (InteractionType) {
	case EHHM_InteractionType::Type_Interaction_SelfToTile:
		Interact_EntityToTile(_data_Interaction, _pInteractionHandler_Executer);
		break;
	case EHHM_InteractionType::Type_Debug_Item_Get_From_Tile:
		Interact_Debug_GetItemFromTile(_data_Interaction, _pInteractionHandler_Executer);
		break;
	default:
		//Exception
		break;
	}
}

void AHHM_Manager_Interaction::Interact_EntityToTile(FHHM_Data_Interaction& _data_Interaction, UHHM_Component_InteractionHandler* _pInteractionHandler_Executer)
{
	//Get Interactor Entity
	if (_pInteractionHandler_Executer == nullptr) {
		//Exception
		return;
	}

	AActor* pActor_Interactor = nullptr;
	pActor_Interactor = _pInteractionHandler_Executer->GetOwner();
	if (pActor_Interactor == nullptr) {
		//Exception
		return;
	}

	AHHM_Entity* pEntity_Interactor = nullptr;
	pEntity_Interactor = Cast<AHHM_Entity>(pActor_Interactor);
	if (pEntity_Interactor == nullptr) {
		//EXception
		return;
	}

	

	//Get Target Tile
	ALocalMap* pLocalMap = nullptr;
	pLocalMap = pEntity_Interactor->Get_LocalMap();
	if (pLocalMap == nullptr) {
		//Exception
		return;
	}

	FVector InteractLocation = _data_Interaction.m_Target_Interaction_Location;
	FVector2D InteractLocation2D = FVector2D(InteractLocation.X, InteractLocation.Z);
	int32 TileIndex = -1;
	bool IsSucceed_ConvertLocation = pLocalMap->Convert_TileIndexLocation_To_Index(TileIndex, InteractLocation2D);
	if (IsSucceed_ConvertLocation == false) {
		//Exception
		return;
	}

	pLocalMap->Interact_Tile(TileIndex, pEntity_Interactor, &_data_Interaction.m_Data_Interact);
}

void AHHM_Manager_Interaction::Interact_Debug_GetItemFromTile(FHHM_Data_Interaction& _data_Interaction, UHHM_Component_InteractionHandler* _pInteractionHandler_Executer)
{
	// HHM Note : Root 인벤토리는 List형 인벤토리로 설정해두고 Root 인벤토리에 아이템이 일정 갯수가 넘어갈경우 포화상태가 되어 root 인벤토리 정리를 1순위 작업으로 하게끔 하기.
	// 포화상태가 아닐경우 하던일을 모두 마치고 다른 일을 시작하기 전이나 일이없을때 주섬주섬 정리함.

	// HHM Note : 아이템이 목적지 (엔티티, 타일위치 등의 정보와 목표 인벤토리 이름과 인덱스 등)를 갖고 목적지를 토대로 인벤토리 경로등을 반영한 액션을 제작하여 실행.
	// UI에서 실행 가능한 "Move Now" 명령을 만들고 아이템을 이동하게끔 플레이어가 드래그를 통해 명령을 하면 목적지 세팅, 해당 공간 예약 등의 작업 진행 후 Move Now를 실행하여 아이템 이동 액션 시작
	// 쉬프트를 누른채 드래그 할경우 이동 마킹만 되어 시간이 될때 알아서 이동하는 식.
	


	//Get Interactor Entity
	if (_pInteractionHandler_Executer == nullptr) {
		//Exception
		return;
	}

	AActor* pActor_Interactor = nullptr;
	pActor_Interactor = _pInteractionHandler_Executer->GetOwner();
	if (pActor_Interactor == nullptr) {
		//Exception
		return;
	}

	AHHM_Entity* pEntity_Interactor = nullptr;
	pEntity_Interactor = Cast<AHHM_Entity>(pActor_Interactor);
	if (pEntity_Interactor == nullptr) {
		//EXception
		return;
	}



	//Get Target Tile Entity
	ALocalMap* pLocalMap = nullptr;
	pLocalMap = pEntity_Interactor->Get_LocalMap();
	if (pLocalMap == nullptr) {
		//Exception
		return;
	}

	FVector InteractLocation = _data_Interaction.m_Target_Interaction_Location;
	FVector2D InteractLocation2D = FVector2D(InteractLocation.X, InteractLocation.Z);
	int32 TileIndex = -1;
	bool IsSucceed_ConvertLocation = pLocalMap->Convert_TileIndexLocation_To_Index(TileIndex, InteractLocation2D);
	if (IsSucceed_ConvertLocation == false) {
		//Exception
		return;
	}

	//FHHM_TileData* pTileData = nullptr;
	//pTileData = pLocalMap->Get_TileData_At(InteractLocation.X, InteractLocation.Z);
	//if (pTileData == nullptr) {
	//	//Exception
	//	return;
	//}

	AHHM_TileEntity* pTileEntity_Raw = nullptr;
	bool IsSucceed_Get_TileEntity = pLocalMap->Get_TileEntity_At(pTileEntity_Raw, InteractLocation.X, InteractLocation.Z);
	if (IsSucceed_Get_TileEntity == false || pTileEntity_Raw == nullptr) {
		//Exception
		return;
	}

	AHHM_TileEntity_ItemContainer* pTileEntity = nullptr;
	pTileEntity = Cast<AHHM_TileEntity_ItemContainer>(pTileEntity_Raw);
	if (pTileEntity == nullptr) {
		//Exception
		return;
	}



	// Check If Tile Has Item.
	UHHM_Inventory_Grid* pInventory = pTileEntity->Get_Inventory_Debug();
	if (pInventory == nullptr) {
		//Exception
		return;
	}

	TMap<int32, FHHM_Data_Inventory_Item>& ItemDataContainer_Ref = pInventory->Get_ItemDataContainer_Ref();
	int32 Num_Item = ItemDataContainer_Ref.Num();
	if (Num_Item < 1) {
		//Exception
		return;
	}



	// Check if Entity has enough room for item to insert
	UHHM_Component_Inventory* pComponent_Inventory = nullptr;
	pComponent_Inventory = pEntity_Interactor->Get_Component_Inventory();
	if (pComponent_Inventory == nullptr) {
		//Exception
		return;
	}

	bool IsItemInsertable = pComponent_Inventory->Check_IsItemInsertable(0, ItemDataContainer_Ref[0].pItemData);
	if (IsItemInsertable == false) {
		return;
	}

	

	//Insert item at entity
	UHHM_ItemData* pItemData_Move = nullptr;
	pItemData_Move = ItemDataContainer_Ref[0].pItemData;
	if (pItemData_Move == nullptr) {
		//Exception
		return;
	}

	int32 InventoryID = 0;
	int32 InventoryItemID = 0;
	EHHM_InventoryReturn Insert_Return = pComponent_Inventory->Item_Insert(InventoryID, InventoryItemID, pItemData_Move);
	if (Insert_Return != EHHM_InventoryReturn::Return_Succeed) {
		//Exception
		return;
	}

	bool IsSucceed_ItemDelete = pTileEntity->Inventory_Item_Delete(pEntity_Interactor, pItemData_Move);
	if (IsSucceed_ItemDelete == false) {
		//Exception
		return;
	}
}
