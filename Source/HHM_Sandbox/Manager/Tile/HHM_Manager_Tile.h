// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/HHM_Manager.h"

#include "Header/Struct_Tile.h"

#include "HHM_Manager_Tile.generated.h"

class AHHM_Tile;

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API AHHM_Manager_Tile : public AHHM_Manager
{
	GENERATED_BODY()

public:
	AHHM_Manager_Tile();

protected:
	virtual void BeginPlay() override;



private:
	UPROPERTY()
		TArray<AHHM_Tile*>		m_Arr_Tile;
	UPROPERTY(VisibleAnywhere)
		TMap<int32, AHHM_Tile*>	m_Container_Tile;

public:
	int32	Register_Tile(AHHM_Tile* pTile);	//may will be removed
private:
	void	Register_All_Tile(void);

public:
	AHHM_Tile*	Get_Tile_By_ID(int32 TileID);
	const FHHM_TileData&	Get_DefaultTileInfo_ByID(int32 TileID);
	
public:
	TMap<int32, AHHM_Tile*>&		Get_TileArr_Ref(void) { return m_Container_Tile; }
};
