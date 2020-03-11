// Fill out your copyright notice in the Description page of Project Settings.

#include "HHM_Tile.h"

// Sets default values
AHHM_Tile::AHHM_Tile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHHM_Tile::BeginPlay()
{
	Super::BeginPlay();
	
	m_DefaultTileData = FHHM_TileData(this, m_BaseTileData);
}

// Called every frame
void AHHM_Tile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



FHHM_TileData AHHM_Tile::On_Placed(ALocalMap * pLocalMap, FHHM_TileData& tileInfo, AEntity * pEntity, const FHHM_MapInfo & mapInfo)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_DefaultTileData;
}

int32 AHHM_Tile::On_Damaged(class ALocalMap* pLocalMap, FHHM_TileData& tileInfo, int32 damage, EHHM_DamageType damage_Type, class APawn* pAttackPawn, const FHHM_MapInfo& mapInfo) {
	int32 Health_Return = tileInfo.HP - damage;
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("TileDamaged"));
	}
	return Health_Return;
}

FHHM_TileData AHHM_Tile::On_Destruct(ALocalMap* pLocalMap, FHHM_TileData& tileInfo, const FHHM_MapInfo& mapInfo) {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Tile Destructed"));
	}

	return m_pManager_Tile->Get_DefaultTileInfo_ByID(0);
	//return FHHM_TileInfo();
}



bool AHHM_Tile::Update_Render(int32& index_instance, FTransform& transform_Local, class ALocalMap* pLocalMap, const FHHM_TileData& tileInfo) {
	index_instance = 0;
	transform_Local = FTransform();
	return true;
}