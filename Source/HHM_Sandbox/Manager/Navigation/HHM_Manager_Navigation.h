// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/HHM_Manager.h"

#include "Header/Struct.h"
#include "Header/Struct_Tile.h"
#include "Header/Struct_Navigation.h"
#include "Header/Struct_Pathfinder.h"

#include "Data/LocalMap/LocalMap.h"

#include "HHM_Manager_Navigation.generated.h"

class AHHM_NavLinkProxy;

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API AHHM_Manager_Navigation : public AHHM_Manager
{
	GENERATED_BODY()

public:
	AHHM_Manager_Navigation();

public:
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;


	
private:
	UPROPERTY()
		TArray<int32>		m_Arr_Region;	//-2 = Not Initialized, -1 = Blocked , 0~ = Region
private:	//NavLink
	UPROPERTY()
		TArray<int32>				m_Arr_PendingQueue_LinkUpdate;	//waiting queue to be updated
	UPROPERTY()
		TArray<AHHM_NavLinkProxy*>	m_Arr_NavLink;
	UPROPERTY()
		TArray<FHHM_NavLinkInfo>	m_Arr_NavLinkInfo;



public:	//Debug Function
	UFUNCTION(BlueprintCallable)
		TArray<int32>	Get_Region() { return m_Arr_Region; }
	UFUNCTION(BlueprintCallable)
		void			Debug_Refresh_Region() { Refresh_Region(); }
	UFUNCTION(BlueprintCallable)
		void			Debug_Refresh_NavLink() { Refresh_NavLink(); }

public:
	UFUNCTION(BlueprintCallable)
		TArray<FHHM_PathNodeData> Search_Path(const ALocalMap* _pLocalMap, FVector2D _location_Start, FVector2D _location_End, int32 _entitySize_Width, int32 _entitySize_Height, int32 _maxJumpLength, int32 _maxFallLength, int32 _maxHorizontalJumpLength) const;



private:
	void	Refresh_NavLink(void);
	void	Update_NavLink(void);
	bool	Check_IsLinkableTile(const TArray<FHHM_TileData>& mapData, const FHHM_MapInfo& mapInfo, int32 checkTileIndex);		//Check tile is linkable state. for put pending queue
	bool	Check_IsTargetableTile(const TArray<FHHM_TileData>& mapData, const FHHM_MapInfo& mapInfo, int32 checkTileIndex);	//Check tile is target-linkable state. basically same as Check_IsLinkableTile function but for check target tile.
	bool	Check_IsReachableTile(const TArray<FHHM_TileData>& mapData, const FHHM_MapInfo& mapInfo, int32 originTileIndex, int32 destTileIndex);	//Check DestTile is Reachable from originTile
	bool	Find_LinkableTile(TArray<int32>* pLinkableTiles, int32 originIndex, const TArray<FHHM_TileData>& mapData, const FHHM_MapInfo& mapInfo);	//return true if find at least one linkable tile index
	bool	Link_Tile(int32* pNavLinkIndex, const int32& originTileIndex, const int32& destTileIndex, const FHHM_MapInfo& mapInfo);

	void	Refresh_Region(void);
	void	Fill_EmptyRegion(const FHHM_MapInfo& MapInfo, int32 StartIndex);	//For Refresh_Region function
};
