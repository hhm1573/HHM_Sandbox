// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_GameMode_LocalMap.h"

#include "Engine/World.h"

#include "Manager/Tile/HHM_Manager_Tile.h"
#include "Manager/Navigation/HHM_Manager_Navigation.h"
#include "Header/Struct_LocalMap.h"
#include "Manager/LocalMap/HHM_Manager_LocalMap.h"
#include "Data/LocalMap/LocalMap.h"

#include "Base/TouchPanel/TouchPanel.h"



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

	if (m_pManager_Tile == nullptr) {
		//m_pManager_Tile = NewObject<AHHM_Manager_Tile>(this, TEXT("Manager_Tile"));
		m_pManager_Tile = pWorld->SpawnActor<AHHM_Manager_Tile>();
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
}



#pragma region Utilities

bool AHHM_GameMode_LocalMap::Utility_Index_Seperate(FVector2D& _vec_Return, int32 _index) {
	int32 Index_Horizontal = _index % m_MapInfo.MapSize_Horizontal;
	int32 Index_Vertical = _index / m_MapInfo.MapSize_Horizontal;

	bool IsValidIndexLocation = AHHM_GameMode_LocalMap::Check_IsValidIndex(Index_Horizontal, Index_Vertical);
	if (IsValidIndexLocation == false) {
		return false;
	}

	FVector2D Vec_Return = FVector2D(Index_Horizontal, Index_Vertical);
	_vec_Return = Vec_Return;

	return true;
}

bool AHHM_GameMode_LocalMap::Utility_Index_Combine(int32& _index_Return, FVector2D _vec_Index) {
	int32 Index_Combined = int32(_vec_Index.X) + (int32(_vec_Index.Y) * m_MapInfo.MapSize_Horizontal);
	
	bool IsValidIndex = AHHM_GameMode_LocalMap::Check_IsValidIndex(Index_Combined);
	if (IsValidIndex == false) {
		return false;
	}

	_index_Return = Index_Combined;

	return true;
}

bool AHHM_GameMode_LocalMap::Utility_Calculate_IndexLocation(FVector2D& _vec_Return, float _location_X, float _location_Z) {
	int32 Index_Horizontal = int32(_location_X / m_MapInfo.TileSize);
	int32 Index_Vertical = int32(_location_Z / m_MapInfo.TileSize);

	FVector2D Vec_Return = FVector2D(Index_Horizontal, Index_Vertical);
	bool IsValidLocation = AHHM_GameMode_LocalMap::Check_IsValidIndex(Vec_Return.X, Vec_Return.Y);
	if (IsValidLocation == false)
		return false;

	_vec_Return = Vec_Return;

	return true;
}

bool AHHM_GameMode_LocalMap::Utility_Calculate_Location(FVector& _vec_Return, FVector2D _vec_IndexLocation, float _location_Y) {
	bool IsValidIndexLocation = AHHM_GameMode_LocalMap::Check_IsValidIndex(_vec_IndexLocation.X, _vec_IndexLocation.Y);
	if (IsValidIndexLocation == false) {
		return false;
	}

	float Location_X = _vec_IndexLocation.X * m_MapInfo.TileSize + ( m_MapInfo.TileSize * 0.5f );
	float Location_Z = _vec_IndexLocation.Y * m_MapInfo.TileSize + ( m_MapInfo.TileSize * 0.5f );
	FVector Vec_Return = FVector(Location_X, _location_Y, Location_Z);
	
	_vec_Return = Vec_Return;

	return true;
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