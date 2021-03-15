// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/HHM_Manager.h"

#include "Header/Struct_Tile.h"
#include "Data/Tile/HHM_Container_SubTile.h"

#include "HHM_Manager_Tile.generated.h"

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
	UPROPERTY(VisibleAnywhere)
		TMap<int32, FHHM_Container_SubTile>	m_Container_Tile;

private:
	void	Register_All_Tile(void);

public:
	AHHM_Tile*	Get_Tile_By_ID(int32 _tileID, int32 _tileSubID = 0);
	const FHHM_TileData&	Get_DefaultTileInfo_ByID(int32 _tileID, int32 _tileSubID = 0) const;
	
public:
	TMap<int32, FHHM_Container_SubTile>&		Get_TileArr_Ref(void) { return m_Container_Tile; }
};
