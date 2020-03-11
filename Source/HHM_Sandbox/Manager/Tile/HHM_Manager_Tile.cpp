// Fill out your copyright notice in the Description page of Project Settings.

#include "HHM_Manager_Tile.h"

#include "Engine/World.h"

#include "UObject/UObjectIterator.h"
#include "AssetRegistryModule.h"

#include "Tile/Base/HHM_Tile.h"
#include "Tile/Default/HHM_Tile_Air.h"
#include "Tile/Default/HHM_Tile_Stone.h"
#include "Tile/Default/HHM_Tile_Dirt.h"



AHHM_Manager_Tile::AHHM_Manager_Tile(void) {

}

void AHHM_Manager_Tile::BeginPlay(void) {
	Super::BeginPlay();

	Register_All_Tile();

	//FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	//TArray<FAssetData> AssetData;
	//AssetRegistryModule.Get().GetAssetsByClass(UDA_Tile::StaticClass()->GetFName(), AssetData);
	//for (int i = 0; i < AssetData.Num(); ++i) {
	//	UDA_Tile* pTile = Cast<UDA_Tile>(AssetData[i].GetAsset());
	//	m_Arr_TileAsset.Add(pTile);
	//}
}



int32 AHHM_Manager_Tile::Register_Tile(AHHM_Tile* pTile) {
	pTile->Set_Manager_Tile(this);
	return m_Arr_Tile.Add(pTile);
}

void AHHM_Manager_Tile::Register_All_Tile(void) {
	
	//Register_Tile(NewObject<AHHM_Tile_Air>(this, TEXT("Origin_Tile_Air")));
	//Register_Tile(NewObject<AHHM_Tile_Stone>(this, TEXT("Origin_Tile_Stone")));
	//Register_Tile(NewObject<AHHM_Tile_Dirt>(this, TEXT("Origin_Tile_Dirt")));
	UWorld* pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception
		return;
	}

	/*Register_Tile(pWorld->SpawnActor<AHHM_Tile_Air>());
	Register_Tile(pWorld->SpawnActor<AHHM_Tile_Stone>());
	Register_Tile(pWorld->SpawnActor<AHHM_Tile_Dirt>());*/

	for (TObjectIterator<UClass> Iterator; Iterator; ++Iterator) {
		if (Iterator->IsChildOf(AHHM_Tile::StaticClass()) && !Iterator->HasAnyClassFlags(EClassFlags::CLASS_Abstract)) {
			AHHM_Tile* pTile = nullptr;
			UClass* pClass = *Iterator;
			pTile = Cast<AHHM_Tile>(pWorld->SpawnActor(pClass));
			
			int32 TileID = pTile->Get_TileID();
			const bool IsIDTaken = m_Container_Tile.Contains(TileID);
			if (IsIDTaken == true) {
				//Exception
				return;
			}

			m_Container_Tile.Add(TileID, pTile);
		}
	}

	int i = 0;
}



AHHM_Tile* AHHM_Manager_Tile::Get_Tile_By_ID(int32 TileID) {
	const int32 Size_TileArr = m_Container_Tile.Num();
	if (TileID >= Size_TileArr) {
		//Exception
		return nullptr;
	}

	return m_Container_Tile[TileID];
}

const FHHM_TileData& AHHM_Manager_Tile::Get_DefaultTileInfo_ByID(int32 TileID) {
	AHHM_Tile* pTile = nullptr;
	pTile = Get_Tile_By_ID(TileID);
	if (pTile == nullptr) {
		//exception
		return m_Container_Tile[0]->Get_DefaultTileInfo();
	}

	const FHHM_TileData& TileData_Return = pTile->Get_DefaultTileInfo();

	return TileData_Return;
}