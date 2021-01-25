// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_TileEntity.h"

#include "Tile/Base/HHM_Tile.h"
#include "Data/LocalMap/LocalMap.h"



// Sets default values
AHHM_TileEntity::AHHM_TileEntity()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AHHM_TileEntity::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHHM_TileEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AHHM_TileEntity::Initialize_TileEntity(AHHM_Tile* _pTile, ALocalMap* _pLocalMap, int32 _index_Horizontal, int32 _index_Vertical)
{
	if (_pTile == nullptr) {
		return false;
	}

	if (_pLocalMap == nullptr) {
		return false;
	}

	m_pLocalMap = _pLocalMap;
	m_Tile_ID = _pTile->Get_TileID();
	m_Tile_SubID = _pTile->Get_TileSubID();
	m_Index_Horizontal = _index_Horizontal;
	m_Index_Vertical = _index_Vertical;



	if (m_Tile_ID < 0) {
		//Exception TileID is negative value
	}
	if (m_Tile_SubID < 0) {
		//Exception TileSubID is negative value
	}



	return true;
}

