// Fill out your copyright notice in the Description page of Project Settings.

#include "Generator_LocalMap.h"

#include "Manager/Tile/HHM_Manager_Tile.h"
#include "Manager/Math/Grid/HHM_Manager_Math_Grid.h"



// Sets default values
AGenerator_LocalMap::AGenerator_LocalMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGenerator_LocalMap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGenerator_LocalMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AGenerator_LocalMap::Generate_DebugMap(TArray<FHHM_TileData>& mapData, const FHHM_MapInfo& mapInfo, const AHHM_Manager_Tile* pManager_Tile) {
	FHHM_TileData TileInfo_Air = pManager_Tile->Get_DefaultTileInfo_ByID(0);
	FHHM_TileData TileInfo_Floor = pManager_Tile->Get_DefaultTileInfo_ByID(1);

	int32 Num_TotalTile = mapInfo.MapSize_Horizontal * mapInfo.MapSize_Vertical;
	mapData.Init(TileInfo_Air, Num_TotalTile);

	//Generate Floor
	for (int32 CurIndex = 0; CurIndex < mapInfo.MapSize_Horizontal; ++CurIndex) {
		mapData[CurIndex] = TileInfo_Floor;
	}

	//mapData[1 + mapInfo.MapSize_Horizontal] = TileInfo_Floor;
	if (mapInfo.MapSize_Vertical >= 1) {
		for (int32 CurIndex = 1; CurIndex < mapInfo.MapSize_Horizontal; CurIndex += 2) {
			mapData[CurIndex + mapInfo.MapSize_Horizontal] = TileInfo_Floor;
		}
	}

	if (mapInfo.MapSize_Vertical >= 3) {
		for (int32 CurIndex = 2; CurIndex < mapInfo.MapSize_Horizontal; CurIndex += 4) {
			mapData[CurIndex + (mapInfo.MapSize_Horizontal * 3)] = TileInfo_Floor;
			mapData[CurIndex + (mapInfo.MapSize_Horizontal * 3) + 1] = TileInfo_Floor;
			mapData[CurIndex + (mapInfo.MapSize_Horizontal * 3) + 2] = TileInfo_Floor;
		}
	}

	if (mapInfo.MapSize_Vertical >= 6) {
		for (int32 CurIndex = 0; CurIndex < mapInfo.MapSize_Horizontal; ++CurIndex) {
			mapData[CurIndex + (mapInfo.MapSize_Horizontal * 6)] = TileInfo_Floor;
		}
	}

	if (mapInfo.MapSize_Vertical >= 9 && mapInfo.MapSize_Horizontal >= 9) {
		mapData[2 + (mapInfo.MapSize_Horizontal * 7)] = TileInfo_Floor;
		mapData[3 + (mapInfo.MapSize_Horizontal * 7)] = TileInfo_Floor;
		mapData[4 + (mapInfo.MapSize_Horizontal * 7)] = TileInfo_Floor;
		
		mapData[7 + (mapInfo.MapSize_Horizontal * 7)] = TileInfo_Floor;
		mapData[8 + (mapInfo.MapSize_Horizontal * 7)] = TileInfo_Floor;
		mapData[9 + (mapInfo.MapSize_Horizontal * 7)] = TileInfo_Floor;
		mapData[7 + (mapInfo.MapSize_Horizontal * 8)] = TileInfo_Floor;
		mapData[8 + (mapInfo.MapSize_Horizontal * 8)] = TileInfo_Floor;
		mapData[9 + (mapInfo.MapSize_Horizontal * 8)] = TileInfo_Floor;

		mapData[12 + (mapInfo.MapSize_Horizontal * 7)] = TileInfo_Floor;
		mapData[13 + (mapInfo.MapSize_Horizontal * 7)] = TileInfo_Floor;
		mapData[14 + (mapInfo.MapSize_Horizontal * 7)] = TileInfo_Floor;
		mapData[12 + (mapInfo.MapSize_Horizontal * 8)] = TileInfo_Floor;
		mapData[13 + (mapInfo.MapSize_Horizontal * 8)] = TileInfo_Floor;
		mapData[14 + (mapInfo.MapSize_Horizontal * 8)] = TileInfo_Floor;
		mapData[12 + (mapInfo.MapSize_Horizontal * 9)] = TileInfo_Floor;
		mapData[13 + (mapInfo.MapSize_Horizontal * 9)] = TileInfo_Floor;
		mapData[14 + (mapInfo.MapSize_Horizontal * 9)] = TileInfo_Floor;
		//mapData[14 + (mapInfo.MapSize_Horizontal * 10)] = TileInfo_Floor;

		mapData[16 + (mapInfo.MapSize_Horizontal * 7)] = TileInfo_Floor;
		mapData[17 + (mapInfo.MapSize_Horizontal * 7)] = TileInfo_Floor;
		mapData[18 + (mapInfo.MapSize_Horizontal * 7)] = TileInfo_Floor;
	}

	mapData[23 + (mapInfo.MapSize_Horizontal * 8)] = TileInfo_Floor;
	mapData[24 + (mapInfo.MapSize_Horizontal * 8)] = TileInfo_Floor;
	mapData[25 + (mapInfo.MapSize_Horizontal * 8)] = TileInfo_Floor;
	mapData[26 + (mapInfo.MapSize_Horizontal * 8)] = TileInfo_Floor;
	//mapData[27 + (mapInfo.MapSize_Horizontal * 8)] = TileInfo_Floor;
	mapData[207] = TileInfo_Air;



	mapData[93] = TileInfo_Air;
	mapData[92] = TileInfo_Air;
	mapData[90] = TileInfo_Floor;

	//mapData[117] = TileInfo_Air;
	//mapData[87] = TileInfo_Floor;

	////Generate a Wall
	//for (int32 CurIndex_Horizontal = 4; CurIndex_Horizontal < mapInfo.MapSize_Horizontal; CurIndex_Horizontal += 4) {
	//	for (int32 CurIndex_Vertical = 0; CurIndex_Vertical < mapInfo.MapSize_Vertical; ++CurIndex_Vertical) {
	//		mapData[AHHM_Manager_Math_Grid::Index_Combine(CurIndex_Horizontal, CurIndex_Vertical, mapInfo)] = TileInfo_Floor;
	//	}
	//}


}