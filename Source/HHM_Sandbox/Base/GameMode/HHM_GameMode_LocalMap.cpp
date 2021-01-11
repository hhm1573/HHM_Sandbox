// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_GameMode_LocalMap.h"

#include "Engine/World.h"

#include "Base/Provider/HHM_Provider_Entity.h"

#include "Manager/Tile/HHM_Manager_Tile.h"
#include "Manager/Navigation/HHM_Manager_Navigation.h"
#include "Header/Struct_LocalMap.h"
#include "Manager/LocalMap/HHM_Manager_LocalMap.h"
#include "Data/LocalMap/LocalMap.h"
#include "Manager/Item/HHM_Manager_Item.h"

#include "Base/TouchPanel/TouchPanel.h"

#include "Header/Struct.h"



AHHM_GameMode_LocalMap::AHHM_GameMode_LocalMap() {
	m_MapInfo.MapSize_Horizontal = 30;
	m_MapInfo.MapSize_Vertical = 15;
	m_MapInfo.TileSize = 10.0f;
}



void AHHM_GameMode_LocalMap::BeginPlay() {
	UWorld* pWorld = nullptr;
	pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception
		return;
	}



	if (m_pProvider_Entity == nullptr) {
		//m_pProvider_Entity = pWorld->SpawnActor<UHHM_Provider_Entity>();
		m_pProvider_Entity = NewObject<UHHM_Provider_Entity>(this, TEXT("Provider_Entity"));
	}



	if (m_pManager_Tile == nullptr) {
		//m_pManager_Tile = NewObject<AHHM_Manager_Tile>(this, TEXT("Manager_Tile"));
		m_pManager_Tile = pWorld->SpawnActor<AHHM_Manager_Tile>();
	}

	if (m_pManager_Item == nullptr) {
		m_pManager_Item = pWorld->SpawnActor<AHHM_Manager_Item>();
	}

	if (m_pManager_Navigation == nullptr) {
		m_pManager_Navigation = pWorld->SpawnActor<AHHM_Manager_Navigation>();
	}

	if (m_pManager_LocalMap == nullptr) {
		m_pManager_LocalMap = pWorld->SpawnActor<AHHM_Manager_LocalMap>();
		//HHM Note : Debug Map

		FHHM_LocalMapConstructionData MapConstructData = FHHM_LocalMapConstructionData(m_MapInfo, EHHM_LocalMap_Option_Generator::LocalMapGenerator_TestMap);
		FHHM_LocalMapConstructionResult MapConstructResult = FHHM_LocalMapConstructionResult();
		
		bool IsSucceed_CreateLocalMap = m_pManager_LocalMap->Create_LocalMap(MapConstructData, MapConstructResult);
		if (IsSucceed_CreateLocalMap == false) {
			//Exception
		}
	}



	Super::BeginPlay();
}



#pragma region Utilities

//bool AHHM_GameMode_LocalMap::Utility_Index_Seperate(FVector2D& _vec_Return, int32 _index) {
//	int32 Index_Horizontal = _index % m_MapInfo.MapSize_Horizontal;
//	int32 Index_Vertical = _index / m_MapInfo.MapSize_Horizontal;
//
//	bool IsValidIndexLocation = AHHM_GameMode_LocalMap::Check_IsValidIndex(Index_Horizontal, Index_Vertical);
//	if (IsValidIndexLocation == false) {
//		return false;
//	}
//
//	FVector2D Vec_Return = FVector2D(Index_Horizontal, Index_Vertical);
//	_vec_Return = Vec_Return;
//
//	return true;
//}
//
//bool AHHM_GameMode_LocalMap::Utility_Index_Combine(int32& _index_Return, FVector2D _vec_Index) {
//	int32 Index_Combined = int32(_vec_Index.X) + (int32(_vec_Index.Y) * m_MapInfo.MapSize_Horizontal);
//	
//	bool IsValidIndex = AHHM_GameMode_LocalMap::Check_IsValidIndex(Index_Combined);
//	if (IsValidIndex == false) {
//		return false;
//	}
//
//	_index_Return = Index_Combined;
//
//	return true;
//}
//
//bool AHHM_GameMode_LocalMap::Utility_Calculate_IndexLocation(FVector2D& _vec_Return, float _location_X, float _location_Z) {
//	int32 Index_Horizontal = int32(_location_X / m_MapInfo.TileSize);
//	int32 Index_Vertical = int32(_location_Z / m_MapInfo.TileSize);
//
//	FVector2D Vec_Return = FVector2D(Index_Horizontal, Index_Vertical);
//	bool IsValidLocation = AHHM_GameMode_LocalMap::Check_IsValidIndex(Vec_Return.X, Vec_Return.Y);
//	if (IsValidLocation == false)
//		return false;
//
//	_vec_Return = Vec_Return;
//
//	return true;
//}
//
//bool AHHM_GameMode_LocalMap::Utility_Calculate_Location(FVector& _vec_Return, FVector2D _vec_IndexLocation, float _location_Y) {
//	bool IsValidIndexLocation = AHHM_GameMode_LocalMap::Check_IsValidIndex(_vec_IndexLocation.X, _vec_IndexLocation.Y);
//	if (IsValidIndexLocation == false) {
//		return false;
//	}
//
//	float Location_X = _vec_IndexLocation.X * m_MapInfo.TileSize + ( m_MapInfo.TileSize * 0.5f );
//	float Location_Z = _vec_IndexLocation.Y * m_MapInfo.TileSize + ( m_MapInfo.TileSize * 0.5f );
//	FVector Vec_Return = FVector(Location_X, _location_Y, Location_Z);
//	
//	_vec_Return = Vec_Return;
//
//	return true;
//}

#pragma endregion



#pragma region Entity Handling

AHHM_Entity* AHHM_GameMode_LocalMap::Spawn_Entity_By_ID(int32 _id_Entity, int32 _id_Map_Horizontal, int32 _id_Map_Vertical, int32 _index_Horizontal, int32 _index_Vertical, float _offset_Depth)
{
	int32 ID_Map = (_id_Map_Vertical * HHM_NUM_MAX_LOCALMAP_IN_ROW) + _id_Map_Horizontal;
	ALocalMap* pLocalMap = nullptr;
	pLocalMap = AHHM_GameMode_LocalMap::Get_LocalMap_ByIndex(ID_Map);
	if (pLocalMap == nullptr) {
		//Exception Caution No LocalMap
		return nullptr;
	}

	//if Index is too far from map border, cancel Spawn
	const FHHM_MapInfo& MapInfo = pLocalMap->Get_MapInfo_ConstRef();
	bool IsTooFar_Left = _index_Horizontal < -HHM_INDEX_MAX_ACCEPTABLE_FROM_MAP_BORDER ? true : false;
	bool IsTooFar_Right = _index_Horizontal - MapInfo.MapSize_Horizontal >= HHM_INDEX_MAX_ACCEPTABLE_FROM_MAP_BORDER ? true : false;
	bool IsTooFar_Down = _index_Vertical < -HHM_INDEX_MAX_ACCEPTABLE_FROM_MAP_BORDER ? true : false;
	bool IsTooFar_Up = _index_Vertical - MapInfo.MapSize_Vertical >= HHM_INDEX_MAX_ACCEPTABLE_FROM_MAP_BORDER ? true : false;
	if (IsTooFar_Left == true || IsTooFar_Right == true || IsTooFar_Up == true || IsTooFar_Down == true) {
		//Exception Caution Too Far From Map Border
		return nullptr;
	}

	//Calculate Location
	FVector Vec_Offset_LocalMap = FVector(_id_Map_Horizontal * HHM_OFFSET_LOCALMAP, 0.0f, _id_Map_Vertical * HHM_OFFSET_LOCALMAP);
	FVector Vec_Offset_IndexLocation = FVector(_index_Horizontal * HHM_TILE_SIZE, 0.0f, _index_Vertical * HHM_TILE_SIZE);
	FVector Vec_Offset_Depth = FVector(0.0f, _offset_Depth, 0.0f);
	FVector Vec_Location = Vec_Offset_LocalMap + Vec_Offset_IndexLocation + Vec_Offset_Depth;

	//Move to tile center location
	FVector Vec_Offset_TileCenter = FVector(HHM_TILE_SIZE * 0.5f, 0.0f, HHM_TILE_SIZE * 0.5f);
	Vec_Location = Vec_Location + Vec_Offset_TileCenter;

	AHHM_Entity* pEntity_Spawned = m_pProvider_Entity->Spawn_Entity_By_ID(_id_Entity, Vec_Location);
	if (pEntity_Spawned == nullptr) {
		//Exception Danger Entity Spawn Failed
		return nullptr;
	}

	return pEntity_Spawned;
}

AHHM_Entity* AHHM_GameMode_LocalMap::Spawn_Entity_By_ID(int32 _id_Entity, int32 _id_Map, int32 _index, float _offset_Depth)
{
	return nullptr;
}

AHHM_Entity* AHHM_GameMode_LocalMap::Spawn_Entity_By_ID(int32 _id_Entity, int32 _id_Map, int32 _index_Horizontal, int32 _index_Vertical, float _offset_Depth)
{
	int32 ID_Map = _id_Map;
	ALocalMap* pLocalMap = nullptr;
	pLocalMap = AHHM_GameMode_LocalMap::Get_LocalMap_ByIndex(ID_Map);
	if (pLocalMap == nullptr) {
		//Exception Caution No LocalMap
		return nullptr;
	}

	//if Index is too far from map border, cancel Spawn
	const FHHM_MapInfo& MapInfo = pLocalMap->Get_MapInfo_ConstRef();
	bool IsTooFar_Left = _index_Horizontal < -HHM_INDEX_MAX_ACCEPTABLE_FROM_MAP_BORDER ? true : false;
	bool IsTooFar_Right = _index_Horizontal - MapInfo.MapSize_Horizontal >= HHM_INDEX_MAX_ACCEPTABLE_FROM_MAP_BORDER ? true : false;
	bool IsTooFar_Down = _index_Vertical < -HHM_INDEX_MAX_ACCEPTABLE_FROM_MAP_BORDER ? true : false;
	bool IsTooFar_Up = _index_Vertical - MapInfo.MapSize_Vertical >= HHM_INDEX_MAX_ACCEPTABLE_FROM_MAP_BORDER ? true : false;
	if (IsTooFar_Left == true || IsTooFar_Right == true || IsTooFar_Up == true || IsTooFar_Down == true) {
		//Exception Caution Too Far From Map Border
		return nullptr;
	}

	//Seperate MapID
	int32 ID_Map_Horizontal = _id_Map % HHM_NUM_MAX_LOCALMAP_IN_ROW;
	int32 ID_Map_Vertical = _id_Map / HHM_NUM_MAX_LOCALMAP_IN_ROW;

	//Calculate Location
	FVector Vec_Offset_LocalMap = FVector(ID_Map_Horizontal * HHM_OFFSET_LOCALMAP, 0.0f, ID_Map_Vertical * HHM_OFFSET_LOCALMAP);
	FVector Vec_Offset_IndexLocation = FVector(_index_Horizontal * HHM_TILE_SIZE, 0.0f, _index_Vertical * HHM_TILE_SIZE);
	FVector Vec_Offset_Depth = FVector(0.0f, _offset_Depth, 0.0f);
	FVector Vec_Location = Vec_Offset_LocalMap + Vec_Offset_IndexLocation + Vec_Offset_Depth;

	//Move to tile center location
	FVector Vec_Offset_TileCenter = FVector(HHM_TILE_SIZE * 0.5f, 0.0f, HHM_TILE_SIZE * 0.5f);
	Vec_Location = Vec_Location + Vec_Offset_TileCenter;

	AHHM_Entity* pEntity_Spawned = m_pProvider_Entity->Spawn_Entity_By_ID(_id_Entity, Vec_Location);
	if (pEntity_Spawned == nullptr) {
		//Exception Danger Entity Spawn Failed
		return nullptr;
	}

	return pEntity_Spawned;
}



bool AHHM_GameMode_LocalMap::Register_Entity(AHHM_Entity* _pEntity, int32* _pEntityID_Return, ALocalMap** _ppLocalMap_Return, const FVector& _location)
{
	if (_pEntity == nullptr || _pEntityID_Return == nullptr || _ppLocalMap_Return == nullptr) {
		//Exception One of input value is nullptr
		return false;
	}

	ALocalMap* pLocalMap_Found = AHHM_GameMode_LocalMap::Get_LocalMap_ByLocation(_location);
	if (pLocalMap_Found == nullptr) {
		//Exception Can not found local map by given location
		return false;
	}

	

	int32 Registered_ID = pLocalMap_Found->Entity_Register(_pEntity);
	if (Registered_ID < 0) {
		//Exception Registering Entity on localmap failed.
		return false;
	}

	*_pEntityID_Return = Registered_ID;
	*_ppLocalMap_Return = pLocalMap_Found;

	return true;
}

int32 AHHM_GameMode_LocalMap::Get_LocalMap_Index_ByLocation(const FVector& _location)
{
	if (m_pManager_LocalMap == nullptr) {
		//Exception LocalMap Manager is nullptr. Critical error
		return -1;
	}



	if (_location.X < HHM_DISTANCE_MAX_ACCEPTABLE_FROM_MAP_BORDER || _location.Y < HHM_DISTANCE_MAX_ACCEPTABLE_FROM_MAP_BORDER) {
		//Exception Entity is way too far from maps
		return -1;
	}

	//Guessing Index of local map. since there is some gap between maps, calculated(Guessed) index does not always indicates exact index.
	int32 Index_Horizontal_Guessed = _location.X / HHM_OFFSET_LOCALMAP;
	int32 Index_Vertical_Guessed = _location.Y / HHM_OFFSET_LOCALMAP;

	//if location does not in range of next map(Guessed + 1 indexed map), it belongs to Guessed map.
	float Location_Horizontal_Acceptable_Min_NextLocalMap = (Index_Horizontal_Guessed + 1) * HHM_OFFSET_LOCALMAP - HHM_DISTANCE_MAX_ACCEPTABLE_FROM_MAP_BORDER;
	float Location_Vertical_Acceptable_Min_NextLocalMap = (Index_Vertical_Guessed + 1) * HHM_OFFSET_LOCALMAP - HHM_DISTANCE_MAX_ACCEPTABLE_FROM_MAP_BORDER;

	int32 Index_Horizontal = (_location.X >= Location_Horizontal_Acceptable_Min_NextLocalMap) ? Index_Horizontal_Guessed + 1 : Index_Horizontal_Guessed;
	int32 Index_Vertical = (_location.Y >= Location_Vertical_Acceptable_Min_NextLocalMap) ? Index_Vertical_Guessed + 1 : Index_Vertical_Guessed;

	//Check if calculated index is not valid.
	if (Index_Horizontal >= HHM_NUM_MAX_LOCALMAP_IN_ROW || Index_Vertical >= HHM_NUM_MAX_LOCALMAP_IN_ROW) {
		//Exception Calculated index is not valid.
		return -1;
	}

	int32 Index_LocalMap = Index_Horizontal + Index_Vertical * HHM_NUM_MAX_LOCALMAP_IN_ROW;
	ALocalMap* pLocalMap = m_pManager_LocalMap->Get_LocalMap(Index_Horizontal);
	if (pLocalMap == nullptr) {
		//Exception nullptr on calculated index
		return -1;
	}

	//Check location is too far from localmap that calculated index indicates.
	const FHHM_MapInfo& MapInfo = pLocalMap->Get_MapInfo_ConstRef();

	float Location_Horizontal_Acceptable_Max_LocalMap = Index_Horizontal * HHM_OFFSET_LOCALMAP + HHM_DISTANCE_MAX_ACCEPTABLE_FROM_MAP_BORDER;
	float Location_Vertical_Acceptable_Max_LocalMap = Index_Vertical * HHM_OFFSET_LOCALMAP + HHM_DISTANCE_MAX_ACCEPTABLE_FROM_MAP_BORDER;

	bool IsHorizontalLocation_TooFar = (_location.X > Location_Horizontal_Acceptable_Max_LocalMap) ? true : false;
	bool IsVerticalLocation_TooFar = (_location.Y > Location_Vertical_Acceptable_Max_LocalMap) ? true : false;

	if (IsHorizontalLocation_TooFar == true || IsVerticalLocation_TooFar == true) {
		//Exception location is too far from map
		return -1;
	}

	return Index_LocalMap;
}

ALocalMap* AHHM_GameMode_LocalMap::Get_LocalMap_ByLocation(const FVector& _location)
{
	if (m_pManager_LocalMap == nullptr) {
		//Exception LocalMap Manager is nullptr. Critical error
		return nullptr;
	}



	if (_location.X < -HHM_DISTANCE_MAX_ACCEPTABLE_FROM_MAP_BORDER || _location.Y < -HHM_DISTANCE_MAX_ACCEPTABLE_FROM_MAP_BORDER) {
		//Exception Entity is way too far from maps
		return nullptr;
	}

	//Guessing Index of local map. since there is some gap between maps, calculated(Guessed) index does not always indicates exact index.
	int32 Index_Horizontal_Guessed = _location.X / HHM_OFFSET_LOCALMAP;
	int32 Index_Vertical_Guessed = _location.Y / HHM_OFFSET_LOCALMAP;

	//if location does not in range of next map(Guessed + 1 indexed map), it belongs to Guessed map.
	float Location_Horizontal_Acceptable_Min_NextLocalMap = (Index_Horizontal_Guessed + 1) * HHM_OFFSET_LOCALMAP - HHM_DISTANCE_MAX_ACCEPTABLE_FROM_MAP_BORDER;
	float Location_Vertical_Acceptable_Min_NextLocalMap = (Index_Vertical_Guessed + 1) * HHM_OFFSET_LOCALMAP - HHM_DISTANCE_MAX_ACCEPTABLE_FROM_MAP_BORDER;

	int32 Index_Horizontal = (_location.X >= Location_Horizontal_Acceptable_Min_NextLocalMap) ? Index_Horizontal_Guessed + 1 : Index_Horizontal_Guessed;
	int32 Index_Vertical = (_location.Y >= Location_Vertical_Acceptable_Min_NextLocalMap) ? Index_Vertical_Guessed + 1 : Index_Vertical_Guessed;

	//Check if calculated index is not valid.
	if (Index_Horizontal >= HHM_NUM_MAX_LOCALMAP_IN_ROW || Index_Vertical >= HHM_NUM_MAX_LOCALMAP_IN_ROW) {
		//Exception Calculated index is not valid.
		return nullptr;
	}

	int32 Index_LocalMap = Index_Horizontal + Index_Vertical * HHM_NUM_MAX_LOCALMAP_IN_ROW;
	ALocalMap* pLocalMap = m_pManager_LocalMap->Get_LocalMap(Index_Horizontal);
	if (pLocalMap == nullptr) {
		//Exception nullptr on calculated index
		return nullptr;
	}

	//Check location is too far from localmap that calculated index indicates.
	const FHHM_MapInfo& MapInfo = pLocalMap->Get_MapInfo_ConstRef();

	float Location_Horizontal_Acceptable_Max_LocalMap = Index_Horizontal * HHM_OFFSET_LOCALMAP + HHM_DISTANCE_MAX_ACCEPTABLE_FROM_MAP_BORDER + (MapInfo.MapSize_Horizontal * HHM_TILE_SIZE);
	float Location_Vertical_Acceptable_Max_LocalMap = Index_Vertical * HHM_OFFSET_LOCALMAP + HHM_DISTANCE_MAX_ACCEPTABLE_FROM_MAP_BORDER + (MapInfo.MapSize_Vertical * HHM_TILE_SIZE);

	bool IsHorizontalLocation_TooFar = (_location.X > Location_Horizontal_Acceptable_Max_LocalMap) ? true : false;
	bool IsVerticalLocation_TooFar = (_location.Y > Location_Vertical_Acceptable_Max_LocalMap) ? true : false;

	if (IsHorizontalLocation_TooFar == true || IsVerticalLocation_TooFar == true) {
		//Exception location is too far from map
		return nullptr;
	}

	return pLocalMap;
}

ALocalMap* AHHM_GameMode_LocalMap::Get_LocalMap_ByIndex(const int32& _index)
{
	if (m_pManager_LocalMap == nullptr) {
		//Exception LocalMap Manager is nullptr. Critical error
		return nullptr;
	}

	ALocalMap* pLocalMap = m_pManager_LocalMap->Get_LocalMap(_index);
	if (pLocalMap == nullptr) {
		//Exception nullptr on input index.
		return nullptr;
	}

	return pLocalMap;
}

#pragma endregion



#pragma region Valid Check

bool AHHM_GameMode_LocalMap::Check_IsValidIndex(int32 _index) {
	if (_index < 0)
		return false;

	int32 Index_Max = (m_MapInfo.MapSize_Horizontal * m_MapInfo.MapSize_Vertical) - 1;
	return _index > Index_Max ? false : true;
}

bool AHHM_GameMode_LocalMap::Check_IsValidIndex(int32 _index_Horizontal, int32 _index_Vertical) {
	if (_index_Horizontal < 0 || _index_Vertical < 0)
		return false;

	if (_index_Horizontal >= m_MapInfo.MapSize_Horizontal)
		return false;

	if (_index_Vertical >= m_MapInfo.MapSize_Vertical)
		return false;

	return true;
}

#pragma endregion