// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Manager_LocalMap.h"

#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Manager/LocalMap/HHM_Manager_LocalMap.h"

#include "Tile/Default/HHM_Tile_Air.h"

#include "Data/LocalMap/LocalMap.h"

#include "Header/Struct_LocalMap.h"



AHHM_Manager_LocalMap::AHHM_Manager_LocalMap() {

}



void AHHM_Manager_LocalMap::BeginPlay() {
	Super::BeginPlay();

	Initialize_Manager_LocalMap();
}



void AHHM_Manager_LocalMap::Initialize_Manager_LocalMap(void) {
	UWorld* pWorld = nullptr;
	pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception
		return;
	}

	AGameModeBase* pGameMode_Raw = nullptr;
	pGameMode_Raw = pWorld->GetAuthGameMode();
	if (pGameMode_Raw == nullptr) {
		//Exception
		return;
	}

	AHHM_GameMode_LocalMap* pGameMode = nullptr;
	pGameMode = Cast<AHHM_GameMode_LocalMap>(pGameMode_Raw);
	if (pGameMode == nullptr) {
		//Exception
		return;
	}

	m_pManager_LocalMap = pGameMode->Get_Manager_Tile_Const();
	if (m_pManager_LocalMap == nullptr) {
		//Exception
		return;
	}
}

bool AHHM_Manager_LocalMap::Create_LocalMap(FHHM_LocalMapConstructionData _mapConstructionData, FHHM_LocalMapConstructionResult& _mapConstructionResult)
{
	if (_mapConstructionData.MapInfo.MapSize_Horizontal <= 0 || _mapConstructionData.MapInfo.MapSize_Vertical <= 0) {
		//Exception input mapinfo is not valid
		return false;
	}



	UWorld* pWorld = nullptr;
	pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception
		return false;
	}



	ALocalMap* pLocalMap_Created = nullptr;
	pLocalMap_Created = pWorld->SpawnActor<ALocalMap>();
	if (pLocalMap_Created == nullptr) {
		//Exception
		return false;
	}

	TArray<FHHM_TileData> Container_TileData;

	switch (_mapConstructionData.GeneratorType)
	{
	case EHHM_LocalMap_Option_Generator::LocalMapGenerator_Empty:
		break;

	case EHHM_LocalMap_Option_Generator::LocalMapGenerator_TestMap:
		break;

	default:
		break;
	}

	return true;
}

bool AHHM_Manager_LocalMap::Initialize_MapData(const FHHM_MapInfo& _mapInfo, int32 _index_LocalMap, TArray<FHHM_TileData> _ref_Container_TileData)
{
	bool IsIndexAvailiable = m_Container_LocalMap.Contains(_index_LocalMap);
	if (IsIndexAvailiable == false) {
		//Exception
		return false;
	}

	int32 Index_Horizontal = _index_LocalMap % HHM_NUM_LOCALMAP_HORIZONTAL;
	int32 Index_Vertical = _index_LocalMap / HHM_NUM_LOCALMAP_HORIZONTAL;

	int32 Num_Container_TileData = _ref_Container_TileData.Num();
	int32 Num_MapTile = _mapInfo.MapSize_Horizontal * _mapInfo.MapSize_Vertical;
	if (Num_Container_TileData != Num_MapTile) {
		//Exception Input MapData's tile number doesnt match with mapinfo
		return false;
	}
	
	m_Container_LocalMap[_index_LocalMap]->Validfy_LocalMap(_index_LocalMap, Index_Horizontal, Index_Vertical, _mapInfo, );

	return true;
}

int32 AHHM_Manager_LocalMap::Find_AvailiableIndex_LocalMap(void)
{
	return int32();
}

void AHHM_Manager_LocalMap::Resize_Container_AvailiableIndex_Entity(void)
{
}

int32 AHHM_Manager_LocalMap::Register_Entity(int32 _index_LocalMap, AHHM_Entity* _pEntity)
{
	return int32();
}

void AHHM_Manager_LocalMap::DeRegister_Entity(int32 _index_LocalMap, AHHM_Entity* _pEntity)
{
}
