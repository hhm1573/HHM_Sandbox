// Fill out your copyright notice in the Description page of Project Settings.

#include "HHM_Manager_Math_Grid.h"

#include "Header/Macro.h"



FVector	AHHM_Manager_Math_Grid::Convert_IndexToTranslation(const FHHM_MapInfo& mapInfo, int32 index) {
	int32	Index_Horizontal = index % mapInfo.MapSize_Horizontal;
	int32	Index_Vertical = index / mapInfo.MapSize_Horizontal;
	FVector	Translation = FVector(Index_Horizontal * HHM_TILE_MESH_SIZE, 0.0f, Index_Vertical * HHM_TILE_MESH_SIZE);
	return Translation;
}

int32	AHHM_Manager_Math_Grid::Convert_TranslationToIndex(const FHHM_MapInfo& mapInfo, const FVector& translation) {
	int32	Index_Horizontal = int32(translation.X / HHM_TILE_MESH_SIZE);
	int32	Index_Vertical = int32(translation.Z / HHM_TILE_MESH_SIZE);
	int32	Index = Index_Horizontal + (Index_Vertical * mapInfo.MapSize_Horizontal);
	return Index;
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