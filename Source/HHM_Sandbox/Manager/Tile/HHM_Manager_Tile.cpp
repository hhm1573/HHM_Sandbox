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



void AHHM_Manager_Tile::Register_All_Tile(void) {
	
	UWorld* pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception
		return;
	}

	for (TObjectIterator<UClass> Iterator; Iterator; ++Iterator) {
		if (Iterator->IsChildOf(AHHM_Tile::StaticClass()) && !Iterator->HasAnyClassFlags(EClassFlags::CLASS_Abstract)) {
			AHHM_Tile* pTile = nullptr;
			UClass* pClass = *Iterator;									// HHM Note : ���� Ÿ�� ���� ��Ŀ����� ID�� �̹� �����Ǿ����� ��� Ÿ���� ������ü�� ���ع�����.
			pTile = Cast<AHHM_Tile>(pWorld->SpawnActor(pClass));		// ���� �پ��� ����� ȣȯ���� ������ ModA-TileA ���� ��ü ID �ý����� ������ �̺κ��� �����ϰ�,
																		// AHHM_Tile ���� m_ID�� ID_Desired�� �����ϰ� ���� ID�� ��ü �ý��ۿ� ������ �������� ���� �����ϰԲ� �� ��
			int32 TileID = pTile->Get_TileID();							// BeginPlay�� DefaultTileData�� �����ϴ� �κ��� Initialize �Լ��� �̵��� �� Initialize �Լ����� ���� ����Ǵ� ID			
			int32 TileSubID = pTile->Get_TileSubID();					// �� �Է��Ҽ� �ְԲ� ����. Ÿ���� ��ȿ�� �˻絵 ���� �ʿ�����.

			if (TileID < 0) {
				//Exception Notice - tile id is negative value
			}
			if (TileSubID < 0) {
				//Exception Notice-tile subid is negative value
			}

			bool IsSubTile = m_Container_Tile.Contains(TileID);
			if (IsSubTile == false) {
				m_Container_Tile.Add(TileID, FHHM_Container_SubTile());
				m_Container_Tile[TileID].Container_SubTile.Empty();
			}
			else {
				bool IsTaken_SubID = m_Container_Tile[TileID].Container_SubTile.Contains(TileSubID);
				if (IsTaken_SubID == true) {
					//Exception Caution-Tile SubID Taken
					pTile->Destroy();
					continue;
				}
			}

			m_Container_Tile[TileID].Container_SubTile.Add(TileSubID, pTile);
		}
	}

	int i = 0;
}



AHHM_Tile* AHHM_Manager_Tile::Get_Tile_By_ID(int32 _tileID, int32 _tileSubID) {
	const bool IsExist_ID = m_Container_Tile.Contains(_tileID);
	if (IsExist_ID == false) {
		//Exception Notice- invalid tile id
		return nullptr;
	}

	const bool IsExist_SubID = m_Container_Tile[_tileID].Container_SubTile.Contains(_tileSubID);
	if (IsExist_SubID == false) {
		//Exception Notice - invalid tile sub id
		return nullptr;
	}

	return m_Container_Tile[_tileID].Container_SubTile[_tileSubID];
}

const FHHM_TileData& AHHM_Manager_Tile::Get_DefaultTileInfo_ByID(int32 _tileID, int32 _tileSubID) const {
	const bool IsExist_ID = m_Container_Tile.Contains(_tileID);
	if (IsExist_ID == false) {
		//Exception Notice - invalid tile id
		return m_Container_Tile[0].Container_SubTile[0]->Get_DefaultTileInfo();
	}

	const bool IsExist_SubID = m_Container_Tile[_tileID].Container_SubTile.Contains(_tileSubID);
	if (IsExist_SubID == false) {
		//Exception Notice - Invalid tile sub id
		return m_Container_Tile[0].Container_SubTile[0]->Get_DefaultTileInfo();
	}

	AHHM_Tile* pTile = m_Container_Tile[_tileID].Container_SubTile[_tileSubID];
	const FHHM_TileData& TileData_Return = pTile->Get_DefaultTileInfo();
	return TileData_Return;
}