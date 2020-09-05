// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Manager_LocalMap.h"

#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Base/GameMode/HHM_GameMode_LocalMap.h"
#include "Manager/Tile/HHM_Manager_Tile.h"

#include "Tile/Base/HHM_Tile.h"
#include "Tile/Default/HHM_Tile_Air.h"

#include "Data/LocalMap/LocalMap.h"

#include "Data/LocalMap/Generator/Generator_LocalMap.h"

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

	m_pManager_Tile = pGameMode->Get_Manager_Tile_Const();
	if (m_pManager_Tile == nullptr) {
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

	FHHM_LocalMap_MapData* pMapData = nullptr;
	pMapData = pLocalMap_Created->Get_MapData_Address();

	//Construct MapData (TileData)
	bool Succeed_Construct_MapData = false;
	switch (_mapConstructionData.GeneratorType)
	{
	case EHHM_LocalMap_Option_Generator::LocalMapGenerator_Empty:
		Succeed_Construct_MapData = AHHM_Manager_LocalMap::Construct_MapData_Empty(pMapData, _mapConstructionData.MapInfo);
		break;

	case EHHM_LocalMap_Option_Generator::LocalMapGenerator_TestMap:
		Succeed_Construct_MapData = AHHM_Manager_LocalMap::Construct_MapData_TestMap(pMapData, _mapConstructionData.MapInfo);
		break;

	default:
		//Exception Unavailiable generator option inserted.
		if (pLocalMap_Created->Destroy() == false) {
			//Exception Created LocalMap is indestructive at some reason
		}
		return false;
		break;
	}



	//Get ID
	int32 ID_LocalMap_Created = AHHM_Manager_LocalMap::Find_AvailiableIndex_LocalMap();
	if (ID_LocalMap_Created < 0) {
		//Exception Found id was under 0
		if (pLocalMap_Created->Destroy() == false) {
			//Exception Created LocalMap is indestructive at some reason
		}
		return false;
	}
	// HHM Note : this check might already executed on find index function that called avobe.
	bool IsInvalidID = m_Container_LocalMap.Contains(ID_LocalMap_Created);
	if (IsInvalidID == true) {
		//Exception Invalid ID Returned.
		if (pLocalMap_Created->Destroy() == false) {
			//Exception Created LocalMap is Indestructive at some reason
		}
		return false;
	}

	int32 Index_Horizontal = ID_LocalMap_Created % HHM_NUM_LOCALMAP_HORIZONTAL;
	int32 Index_Vertical = ID_LocalMap_Created / HHM_NUM_LOCALMAP_HORIZONTAL;

	float Location_Horizontal = Index_Horizontal * HHM_OFFSET_LOCALMAP;
	float Location_Vertical = Index_Vertical * HHM_OFFSET_LOCALMAP;

	FVector Location_LocalMap = FVector(Location_Horizontal, 0.0f, Location_Vertical);
	pLocalMap_Created->SetActorLocation(Location_LocalMap);

	

	pLocalMap_Created->Validfy_LocalMap(ID_LocalMap_Created, Index_Horizontal, Index_Vertical, _mapConstructionData.MapInfo);

	m_Container_LocalMap.Add(ID_LocalMap_Created, pLocalMap_Created);

	//HHM Note : Log Created LocalMap Result info.

	return true;
}

ALocalMap* AHHM_Manager_LocalMap::Get_LocalMap(int32 _index_LocalMap)
{
	bool IsAvailable_Index = m_Container_LocalMap.Contains(_index_LocalMap);
	return IsAvailable_Index == true ? m_Container_LocalMap[_index_LocalMap] : nullptr;
}

const ALocalMap* AHHM_Manager_LocalMap::Get_LocalMap_const(int32 _index_LocalMap) const
{
	bool IsAvailable_Index = m_Container_LocalMap.Contains(_index_LocalMap);
	return IsAvailable_Index == true ? m_Container_LocalMap[_index_LocalMap] : nullptr;
}



bool AHHM_Manager_LocalMap::Construct_MapData_Empty(FHHM_LocalMap_MapData* _mapData, const FHHM_MapInfo& _mapInfo) {
	_mapData->Container_TileData.Empty();
	
	FHHM_TileData TileData_Air = m_pManager_Tile->Get_DefaultTileInfo_ByID(0);

	int32 Num_TotalTile = _mapInfo.MapSize_Horizontal * _mapInfo.MapSize_Vertical;

	_mapData->Container_TileData.Init(TileData_Air, Num_TotalTile);

	return true;
}

bool AHHM_Manager_LocalMap::Construct_MapData_TestMap(FHHM_LocalMap_MapData* _mapData, const FHHM_MapInfo& _mapInfo) {
	//empty
	//Gen::Create()

	if (m_pManager_Tile == nullptr) {
		//Exception
		return false;
	}

	_mapData->Empty();

	AGenerator_LocalMap::Generate_DebugMap(_mapData->Container_TileData, _mapInfo, m_pManager_Tile);


	return true;
}



int32 AHHM_Manager_LocalMap::Find_AvailiableIndex_LocalMap(void)
{
	//where to spawn localmap(call function of manager?) -> GameMode
	
	bool isStoredIndexAvailiable = !(m_Container_LocalMap.Contains(m_AvailiableIndex_LocalMap));

	if (isStoredIndexAvailiable == true) {
		int32 Index_Return = m_AvailiableIndex_LocalMap;
		++m_AvailiableIndex_LocalMap;

		return Index_Return;
		// Because of local map id won't be reset until stored index failed, it may occur some issue.
		// But since it reset on game restart, that would be extremely rare case.
		// 모든 작업이 옳바르고 의도한대로 작동했을시 멤버변수로 저장된 사용가능 아이디 변수를 초기화하거나 하는 변수가 없기에 
		// 해당 변수가 언젠가는 4바이트 정수변수의 한계에 다다를 가능성이 있다. 
		// 하지만 한번도 게임을 재시작 하지않은 상태로 4바이트 정수변수의 한계치까지 맵을 생성하는경우는 극히 드물것이라 생각되기에 추가적인 작업을 해놓지 않겠다.
	}

	

	//Find Availiable index
	for (int32	Index_Availiable_Found = 0; Index_Availiable_Found < 1000; ++Index_Availiable_Found) {
		bool isIndexAvailiable = !(m_Container_LocalMap.Contains(Index_Availiable_Found));
		if (isIndexAvailiable == true) {
			m_AvailiableIndex_LocalMap = Index_Availiable_Found + 1;
			return Index_Availiable_Found;
		}
	}

	//Exception existing 1000 localmap same time is neally impossible.
	return -1;
}

void AHHM_Manager_LocalMap::Resize_Container_AvailiableIndex_Entity(void)
{	//Move entity job to LocalMap. work on that
}

int32 AHHM_Manager_LocalMap::Register_Entity(int32 _index_LocalMap, AHHM_Entity* _pEntity)	//Move entity container to localmap
{
	return int32();
}

void AHHM_Manager_LocalMap::DeRegister_Entity(int32 _index_LocalMap, AHHM_Entity* _pEntity)
{
}
