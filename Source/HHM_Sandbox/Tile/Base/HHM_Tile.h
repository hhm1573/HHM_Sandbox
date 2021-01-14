// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Header/Struct.h"
#include "Header/Struct_Tile.h"
#include "Header/Enum.h"
#include "Header/Struct_RenderInfo.h"

#include "Manager/Tile/HHM_Manager_Tile.h"

#include "HHM_Tile.generated.h"




UCLASS(Abstract)
class HHM_SANDBOX_API AHHM_Tile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHHM_Tile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



protected:
	UPROPERTY()
		AHHM_Manager_Tile*		m_pManager_Tile = nullptr;

	UPROPERTY()
		int32			m_ID = -1;	//Desired ID for regist tile

	UPROPERTY()
		FHHM_TileData_Base	m_BaseTileData;
	UPROPERTY()
		FHHM_TileData		m_DefaultTileData;
	UPROPERTY()
		FHHM_RenderInfo		m_RenderInfo;

public:		//Getter
	int32					Get_TileID(void) const { return m_ID; }
	bool					Get_IsPassable(void) const { return !m_BaseTileData.IsPassable; }
	int32					Get_MaxHealthPoint(void) const { return m_BaseTileData.HP_Max; }
	const FHHM_TileData&	Get_DefaultTileInfo(void) const { return m_DefaultTileData; }
	const FHHM_RenderInfo&	Get_RenderInfo(void) const { return m_RenderInfo; }

public:		//Setter
	void					Set_Manager_Tile(AHHM_Manager_Tile* pmanager_Tile) { if (pmanager_Tile != nullptr) m_pManager_Tile = pmanager_Tile; }



public:	//LocalMap, TileInfo, ~~~, MapInfo
	virtual FHHM_TileData			On_Placed(class ALocalMap* pLocalMap, FHHM_TileData& tileInfo, class AEntity* pEntity, const FHHM_MapInfo& mapInfo);	//Call when Entity place tile on map
	virtual int32					On_Damaged(class ALocalMap* pLocalMap, FHHM_TileData& tileInfo, int32 damage, EHHM_DamageType damage_Type, class APawn* pAttackPawn, const FHHM_MapInfo& mapInfo);	//Called when tile got damage. return the health point after damaged
	virtual FHHM_TileData			On_Destruct(class ALocalMap* pLocalMap, FHHM_TileData& tileInfo, const FHHM_MapInfo& mapInfo);		//Called when tile destructed. return tile will replace current tile.

	//Update Render Info Based on inserted tileInfo(Data of updating tile). Called from LocalMap whenever tile's data has changed (for example, Damaged)
	virtual bool					Update_Render(int32& index_Instance, FTransform& transform_Local, class ALocalMap* pLocalMap, const FHHM_TileData& tileInfo);
	

};
