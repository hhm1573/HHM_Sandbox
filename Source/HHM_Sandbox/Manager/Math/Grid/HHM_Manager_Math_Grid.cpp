// Fill out your copyright notice in the Description page of Project Settings.

#include "HHM_Manager_Math_Grid.h"

#include "Header/Macro.h"



bool AHHM_Manager_Math_Grid::Convert_Index_To_Translation(FVector& _vec_Translation_Return, int32 _index, const FHHM_MapInfo& _mapInfo) 
{
	if (_mapInfo.MapSize_Horizontal <= 0) {
		//Exception
		return false;
	}

	int32	Index_Horizontal = _index % _mapInfo.MapSize_Horizontal;
	int32	Index_Vertical = _index / _mapInfo.MapSize_Horizontal;
	FVector	Vec_Translation_Return = FVector(Index_Horizontal * HHM_TILE_MESH_SIZE, 0.0f, Index_Vertical * HHM_TILE_MESH_SIZE);
	_vec_Translation_Return = Vec_Translation_Return;
	return true;
}

bool AHHM_Manager_Math_Grid::Convert_IndexLocation_To_Translation(FVector& _vec_Return, const FVector2D& _vec_IndexLocation, const FHHM_MapInfo& _mapInfo)
{
	//HHM Note : 입력된 인덱스 위치값이 맵정보에 입력된 값의 범위 안에 있는지에 대한 검사및 예외처리부분이 필요없다 생각하여 작성되어있지 않습니다. 추후 필요해지게 된다면 추가할 예정입니다.
	FVector Vec_Translation = FVector(_vec_IndexLocation.X * HHM_TILE_MESH_SIZE, _vec_IndexLocation.Y, _vec_IndexLocation.Y * HHM_TILE_MESH_SIZE);
	_vec_Return = Vec_Translation;
	return true;
}

bool AHHM_Manager_Math_Grid::Convert_Translation_To_Index(int32& _index_Return, const FVector& _vec_Translation, const FHHM_MapInfo& _mapInfo) {
	if (_mapInfo.MapSize_Horizontal <= 0) {
		//Exception
		return false;
	}

	int32	Index_Horizontal = int32(_vec_Translation.X / HHM_TILE_MESH_SIZE);
	int32	Index_Vertical = int32(_vec_Translation.Z / HHM_TILE_MESH_SIZE);
	int32	Index_Return = Index_Horizontal + (Index_Vertical * _mapInfo.MapSize_Horizontal);
	_index_Return = Index_Return;
	return true;
}

bool AHHM_Manager_Math_Grid::Convert_Translation_To_IndexLocation(FVector2D& _vec_Return, const FVector& _translation, const FHHM_MapInfo& _mapInfo)
{
	if (_mapInfo.MapSize_Horizontal <= 0) {
		//Exception
		return false;
	}

	int32	Index_Horizontal = int32(_translation.X / HHM_TILE_MESH_SIZE);
	int32	Index_Vertical = int32(_translation.Z / HHM_TILE_MESH_SIZE);
	FVector2D Vec_Return = FVector2D(Index_Horizontal, Index_Vertical);
	_vec_Return = Vec_Return;
	return true;
}



int32	AHHM_Manager_Math_Grid::Index_Combine(int32 _index_Horizontal, int32 _index_Vertical, const FHHM_MapInfo& _mapInfo) {
	int32 Index_Return = _index_Horizontal + (_index_Vertical * _mapInfo.MapSize_Horizontal);
	return Index_Return;
}

bool	AHHM_Manager_Math_Grid::Index_Seperate(int32& _index_Horizontal, int32& _index_Vertical, int32 _index, const FHHM_MapInfo& _mapInfo) {
	int32	Index_Horizontal_Return = _index % _mapInfo.MapSize_Horizontal;
	int32	Index_Vertical_Return = _index / _mapInfo.MapSize_Horizontal;

	if (Index_Horizontal_Return < 0 || Index_Horizontal_Return >= _mapInfo.MapSize_Horizontal
		|| Index_Vertical_Return < 0 || Index_Vertical_Return >= _mapInfo.MapSize_Vertical) {
		return false;
	}

	_index_Horizontal = Index_Horizontal_Return;
	_index_Vertical = Index_Vertical_Return;
	
	return true;
}

bool	AHHM_Manager_Math_Grid::Index_Seperate(FVector2D& _vecLocation_Return, int32 _index, const FHHM_MapInfo& _mapInfo) {
	int32	Index_Horizontal_Return = _index % _mapInfo.MapSize_Horizontal;
	int32	Index_Vertical_Return = _index / _mapInfo.MapSize_Horizontal;

	if (Index_Horizontal_Return < 0 || Index_Horizontal_Return >= _mapInfo.MapSize_Horizontal
		|| Index_Vertical_Return < 0 || Index_Vertical_Return >= _mapInfo.MapSize_Vertical) {
		return false;
	}

	_vecLocation_Return.X = Index_Horizontal_Return;
	_vecLocation_Return.Y = Index_Vertical_Return;

	return true;
}