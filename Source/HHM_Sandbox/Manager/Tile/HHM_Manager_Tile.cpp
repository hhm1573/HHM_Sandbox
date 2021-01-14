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
	
	UWorld* pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception
		return;
	}

	for (TObjectIterator<UClass> Iterator; Iterator; ++Iterator) {
		if (Iterator->IsChildOf(AHHM_Tile::StaticClass()) && !Iterator->HasAnyClassFlags(EClassFlags::CLASS_Abstract)) {
			AHHM_Tile* pTile = nullptr;
			UClass* pClass = *Iterator;
			pTile = Cast<AHHM_Tile>(pWorld->SpawnActor(pClass));
			
			int32 TileID = pTile->Get_TileID();										

			if (TileID < 0) {
				continue;
			}
																					// HHM Note : 현재 타일 생성 방식에서는 ID가 이미 점유되어있을 경우 타일을 생성자체를 안해버린다.
			const bool IsIDTaken = m_Container_Tile.Contains(TileID);				// 추후 다양한 모드의 호환성을 염두해 ModA-TileA 같은 대체 ID 시스템을 만들경우 이부분을 수정하고,
			if (IsIDTaken == true) {												// AHHM_Tile 에서 m_ID를 ID_Desired로 변경하고 실제 ID는 대체 시스템에 따르는 형식으로 따로 저장하게끔 한 후
				//Exception															// BeginPlay에 DefaultTileData를 생성하는 부분을 Initialize 함수로 이동한 후 Initialize 함수에서 실제 저장되는 ID
				pTile->Destroy();													// 를 입력할수 있게끔 설정. 타일의 유효성 검사도 따로 필요해짐.
				continue;
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

const FHHM_TileData& AHHM_Manager_Tile::Get_DefaultTileInfo_ByID(int32 TileID) const {
	const int32 Size_TileArr = m_Container_Tile.Num();
	if (TileID >= Size_TileArr) {
		//Exception input tileID is out of range
		return m_Container_Tile[0]->Get_DefaultTileInfo();
	}

	AHHM_Tile* pTile = m_Container_Tile[TileID];
	if (pTile == nullptr) {
		//exception
		return m_Container_Tile[0]->Get_DefaultTileInfo();
	}

	const FHHM_TileData& TileData_Return = pTile->Get_DefaultTileInfo();

	return TileData_Return;
}