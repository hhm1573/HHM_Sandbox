// Fill out your copyright notice in the Description page of Project Settings.

#include "HHM_Tile.h"

#include "Data/LocalMap/LocalMap.h"



// Sets default values
AHHM_Tile::AHHM_Tile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_DefaultTileData.Tile = this;
}

// Called when the game starts or when spawned
void AHHM_Tile::BeginPlay()
{
	Super::BeginPlay();

	m_DefaultTileData.ID = m_ID;
	m_DefaultTileData.SubID = m_SubID;

	m_DefaultTileData.HP = m_DefaultTileData.HP_Max;
}

// Called every frame
void AHHM_Tile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



bool AHHM_Tile::On_Placed(ALocalMap * pLocalMap, FHHM_TileData& tileInfo, AHHM_Entity * pEntity)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	//return m_DefaultTileData;
	return true;
}

int32 AHHM_Tile::On_Damaged(class ALocalMap* pLocalMap, FHHM_TileData& tileInfo, int32 damage, EHHM_DamageType damage_Type, class APawn* pAttackPawn) {
	int32 Health_Return = tileInfo.HP - damage;
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("TileDamaged"));
	}
	return Health_Return;
}

FHHM_TileData AHHM_Tile::On_Destruct(ALocalMap* pLocalMap, FHHM_TileData& tileInfo) {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Tile Destructed"));
	}

	if (pLocalMap == nullptr) {
		//Exception Input LocalMap Is Nullptr
	}
	else {
		bool IsSucceed_ClearTileEntityData = pLocalMap->Set_TileEntity_At(tileInfo.Index_Horizontal, tileInfo.Index_Vertical, nullptr);
		if (IsSucceed_ClearTileEntityData == false) {
			//Exception Clear TileEntity Data failed
		}
	}

	return m_pManager_Tile->Get_DefaultTileInfo_ByID(0,0);
	//return FHHM_TileInfo();
}

void AHHM_Tile::On_Delete(ALocalMap* _pLocalMap, FHHM_TileData& _tileInfo)
{
	if (_pLocalMap == nullptr) {
		//Exception Input LocalMap Is Nullptr
		return;
	}

	bool IsSucceed_ClearTileEntityData = _pLocalMap->Set_TileEntity_At(_tileInfo.Index_Horizontal, _tileInfo.Index_Vertical, nullptr);
	if (IsSucceed_ClearTileEntityData == false) {
		//Exception Clear TileEntity Data Failed
		return;
	}

	return;
}



bool AHHM_Tile::Update_Render(int32& index_instance, FTransform& transform_Local, class ALocalMap* pLocalMap, const FHHM_TileData& tileInfo) {
	index_instance = 0;
	transform_Local = FTransform();
	return true;
}

bool AHHM_Tile::Data_Set_Ladder(bool _isLadder, float _speedMultiplier)
{
	if (_isLadder == true) {
		if (_speedMultiplier <= 0.0f) {
			//Exception Invalid Speed multiplier value
			return false;
		}
		m_SpeedMultiplier_Ladder = _speedMultiplier;
	}
	else {
		m_SpeedMultiplier_Ladder = 0.0f;
	}

	m_DefaultTileData.IsLadder = _isLadder;

	return true;
}
