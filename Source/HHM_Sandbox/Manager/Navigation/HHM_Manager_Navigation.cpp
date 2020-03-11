// Fill out your copyright notice in the Description page of Project Settings.

#include "HHM_Manager_Navigation.h"

#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Base/GameMode/HHM_GameMode_LocalMap.h"

#include "Header/Struct.h"
#include "Data/LocalMap/LocalMap.h"

#include "Manager/Navigation/NavLink/HHM_NavLinkProxy.h"
#include "Manager/Navigation/NavLink/HHM_NavLinkProxy_H1.h"
#include "Manager/Navigation/NavLink/HHM_NavLinkProxy_H2.h"

#include "Manager/Math/Grid/HHM_Manager_Math_Grid.h"

#include "PathFinder/HHM_PathFinder.h"



AHHM_Manager_Navigation::AHHM_Manager_Navigation(void) {

}

void AHHM_Manager_Navigation::Tick(float DeltaTime) {

}

void AHHM_Manager_Navigation::BeginPlay(void) {
	Super::BeginPlay();
	//Refresh_Region();
}



TArray<FHHM_PathNodeData> AHHM_Manager_Navigation::Search_Path(const ALocalMap* _pLocalMap, FVector2D _location_Start, FVector2D _location_End, int32 _entitySize_Width, int32 _entitySize_Height, int32 _maxJumpLength, int32 _maxFallLength, int32 _maxHorizontalJumpLength) const
{
	return UHHM_PathFinder::Search_Path(_pLocalMap, _location_Start, _location_End, _entitySize_Width, _entitySize_Height, _maxJumpLength, _maxFallLength, _maxHorizontalJumpLength);
}



void AHHM_Manager_Navigation::Refresh_NavLink(void) {
	//Getting MapData
	UWorld* pWorld = nullptr;
	pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception
		return;
	}
	AGameModeBase* pGameMode = nullptr;
	pGameMode = pWorld->GetAuthGameMode();
	if (pGameMode == nullptr) {
		//Exception
		return;
	}
	AHHM_GameMode_LocalMap* pGameMode_LocalMap = nullptr;
	pGameMode_LocalMap = Cast<AHHM_GameMode_LocalMap>(pGameMode);
	if (pGameMode_LocalMap == nullptr) {
		//Exception
		return;
	}
	ALocalMap* pLocalMap = nullptr;
	pLocalMap = pGameMode_LocalMap->Get_LocalMap();
	if (pLocalMap == nullptr) {
		//Exception
		return;
	}

	const TArray<FHHM_TileData>&	MapData = pLocalMap->Get_MapData();
	const FHHM_MapInfo&				MapInfo = pGameMode_LocalMap->Get_MapInfo();
	const int32 MapSize = MapInfo.MapSize_Horizontal * MapInfo.MapSize_Vertical;

	//Empty all data and re-init navlinkinfo array
	const int32 NavLinkNum = m_Arr_NavLink.Num();
	for (int32 CurIndex = 0; CurIndex < NavLinkNum; ++CurIndex) {
		if (m_Arr_NavLink[CurIndex]->Destroy() == false) {
			//Exception May some memory leak happen
		}
	}
	m_Arr_NavLink.Empty();
	m_Arr_NavLinkInfo.Empty();
	m_Arr_PendingQueue_LinkUpdate.Empty();

	m_Arr_NavLinkInfo.Init(FHHM_NavLinkInfo(), MapSize);

	//Create Update pending queue
	for (int32 CurIndex = 0; CurIndex < MapSize; ++CurIndex) {
		if (Check_IsLinkableTile(MapData, MapInfo, CurIndex) == true) {
			m_Arr_PendingQueue_LinkUpdate.Add(CurIndex);
		}
	}

	//iterate pending queue and create NavLinkProxy if it is availiable
	const int32 PendingQueueSize = m_Arr_PendingQueue_LinkUpdate.Num();
	for (int32 CurIndex = 0; CurIndex < PendingQueueSize; ++CurIndex) {
		//Find Linkable tiles
		const int32 OriginTileIndex = m_Arr_PendingQueue_LinkUpdate[CurIndex];
		TArray<int32>	LinkableTiles;

		if (Find_LinkableTile(&LinkableTiles, OriginTileIndex, MapData, MapInfo) == false) {
			continue;	//if there is no linkable tiles continue.
		}

		//iterate linkable tiles and create NavLinkProxy
		const int32 LinkableTileNum = LinkableTiles.Num();
		for (int32 CurIndex_Linkable = 0; CurIndex_Linkable < LinkableTileNum; ++CurIndex_Linkable) {
			const int32 DestTileIndex = LinkableTiles[CurIndex_Linkable];

			//Create NavLinkProxy
			int32 Created_NavLinkIndex = -1;
			if (Link_Tile(&Created_NavLinkIndex, OriginTileIndex, DestTileIndex, MapInfo) == false) {
				//Exception Link failed
				continue;
			}
			if (Created_NavLinkIndex == -1) {
				//Exception
				continue;
			}
			//Save NavLink Info
			m_Arr_NavLinkInfo[OriginTileIndex].Arr_NavLink_Index.Add(Created_NavLinkIndex);
			m_Arr_NavLinkInfo[OriginTileIndex].Arr_NavLink_TargetTileIndex.Add(DestTileIndex);
			m_Arr_NavLinkInfo[DestTileIndex].Arr_TileIndex_TargetThis.Add(OriginTileIndex);
		}
	}

	//Empty Pending queue
	m_Arr_PendingQueue_LinkUpdate.Empty();
	
}

void AHHM_Manager_Navigation::Update_NavLink(void) {

}

bool AHHM_Manager_Navigation::Check_IsLinkableTile(const TArray<FHHM_TileData>& mapData, const FHHM_MapInfo& mapInfo, int32 checkTileIndex) {
	int32 TileIndex_Horizontal = -1;
	int32 TileIndex_Vertical = -1;
	AHHM_Manager_Math_Grid::Index_Seperate(TileIndex_Horizontal, TileIndex_Vertical, checkTileIndex, mapInfo);

	//Check target tile is in availiable location
	if (TileIndex_Horizontal < 0 || TileIndex_Vertical < 0
		|| TileIndex_Horizontal >= mapInfo.MapSize_Horizontal || TileIndex_Vertical >= mapInfo.MapSize_Vertical) {
		return false;
	}

	//Check checking tile is actually block passage
	if (mapData[checkTileIndex].IsPassable == true) {
		return false;
	}

	//Check it located top/right edge of the map
	const int32 EdgeIndex_Right = mapInfo.MapSize_Horizontal - 1;	//HHM Note : make it memeber variable if lack of CPU and enough mem
	const int32 EdgeIndex_Top = mapInfo.MapSize_Vertical - 1;
	if (TileIndex_Horizontal == EdgeIndex_Right || TileIndex_Vertical == EdgeIndex_Top) {
		return false;
	}

	//Check up-tile is blocking passage
	const int32 UpTileIndex = checkTileIndex + mapInfo.MapSize_Horizontal;
	if (mapData[UpTileIndex].IsPassable == false) {
		return false;
	}

	return true;
}

bool AHHM_Manager_Navigation::Check_IsTargetableTile(const TArray<FHHM_TileData>& mapData, const FHHM_MapInfo& mapInfo, int32 checkTileIndex) {

	int32 TileIndex_Horizontal = -1;
	int32 TileIndex_Vertical = -1;
	AHHM_Manager_Math_Grid::Index_Seperate(TileIndex_Horizontal, TileIndex_Vertical, checkTileIndex, mapInfo);

	//Check target tile is in availiable location
	if (TileIndex_Horizontal < 0 || TileIndex_Vertical < 0
		|| TileIndex_Horizontal >= mapInfo.MapSize_Horizontal || TileIndex_Vertical >= mapInfo.MapSize_Vertical) {
		return false;
	}

	//Check checking tile is actually block passage
	if (mapData[checkTileIndex].IsPassable == true) {
		return false;
	}

	//Check it located top edge of the map
	const int32 EdgeIndex_Top = mapInfo.MapSize_Vertical - 1;		//HHM Note : if you want make to player can move over map's max height so need to link over max height, remove this check section and fix below block passage check section.
	if (TileIndex_Vertical == EdgeIndex_Top) {						//HHM Note : kinda same text with above one. 맵의 가장 윗부분의 타일에도 링크를 할 필요가 생길경우, 이 체크부분을 삭제하고, 아래 위 타일 체크 부분에 가장 윗부분에 대한 예외처리를 해야 맵 컨테이너 밖을 조회하고 프로그램이 터지는 일이 없음.
		return false;
	}

	//Check up-tile is blocking passage
	const int32 UpTileIndex = checkTileIndex + mapInfo.MapSize_Horizontal;
	if (mapData[UpTileIndex].IsPassable == false) {
		return false;
	}

	return true;
}

bool AHHM_Manager_Navigation::Check_IsReachableTile(const TArray<FHHM_TileData>& mapData, const FHHM_MapInfo& mapInfo, int32 originTileIndex, int32 destTileIndex) {
	//Seperate Origin and Dest Tile Index
	int32 OriginIndex_Horizontal = -1;
	int32 OriginIndex_Vertical = -1;
	int32 DestIndex_Horizontal = -1;
	int32 DestIndex_Vertical = -1;
	if (AHHM_Manager_Math_Grid::Index_Seperate(OriginIndex_Horizontal, OriginIndex_Vertical, originTileIndex, mapInfo) == false) {
		//Exception
		return false;
	}
	if (AHHM_Manager_Math_Grid::Index_Seperate(DestIndex_Horizontal, DestIndex_Vertical, destTileIndex, mapInfo) == false) {
		//Exception
		return false;
	}

	//since input index is Tile index, add 1 to vertical so make it pointing standing point
	OriginIndex_Vertical += 1;
	DestIndex_Vertical += 1;

	//check dest it above or down and make search information
	int32			Search_Horizontal = -1;
	TArray<int32>	arr_Search_Vertical;
	arr_Search_Vertical.Empty();
	if (DestIndex_Vertical > OriginIndex_Vertical) {
		Search_Horizontal = OriginIndex_Horizontal;
		for (int32 CurIndex_Vertical = OriginIndex_Vertical; CurIndex_Vertical <= DestIndex_Vertical; ++CurIndex_Vertical) {
			arr_Search_Vertical.Add(CurIndex_Vertical);
		}
	}
	else {
		Search_Horizontal = DestIndex_Horizontal;
		for (int32 CurIndex_Vertical = OriginIndex_Vertical; CurIndex_Vertical >= DestIndex_Vertical; --CurIndex_Vertical) {
			arr_Search_Vertical.Add(CurIndex_Vertical);
		}
	}

	//Search obstacle
	for (int32 CurIndex_Search = 0; CurIndex_Search < arr_Search_Vertical.Num(); ++CurIndex_Search) {
		int32 SearchIndex = AHHM_Manager_Math_Grid::Index_Combine(Search_Horizontal, arr_Search_Vertical[CurIndex_Search], mapInfo);
		if (mapData[SearchIndex].IsPassable == false) {
			return false;
		}
	}

	return true;

}

bool AHHM_Manager_Navigation::Find_LinkableTile(TArray<int32>* pLinkableTiles, int32 originIndex, const TArray<FHHM_TileData>& mapData, const FHHM_MapInfo& mapInfo) {
	const int32 SearchHeight = 2;
	bool		bFindLinkableTile = false;

	pLinkableTiles->Empty();

	int32 OriginIndex_Horizontal = -1;
	int32 OriginIndex_Vertical = -1;
	if (AHHM_Manager_Math_Grid::Index_Seperate(OriginIndex_Horizontal, OriginIndex_Vertical, originIndex, mapInfo) == false) {
		//Exception
		return false;
	}
	
	const int32 CheckIndex_Horizontal = OriginIndex_Horizontal + 1;
	const int32 MaxIndex = mapInfo.MapSize_Horizontal * mapInfo.MapSize_Vertical - 1;
	for (int32 CurrentSearchHeight = SearchHeight * -1; CurrentSearchHeight <= SearchHeight; ++CurrentSearchHeight) {
		//check is it same height
		if (CurrentSearchHeight == 0) {
			continue;
		}

		//Check target tile is linkable state
		int32 CheckIndex_Vertical = OriginIndex_Vertical + CurrentSearchHeight;
		int32 CheckIndex = AHHM_Manager_Math_Grid::Index_Combine(CheckIndex_Horizontal, CheckIndex_Vertical, mapInfo);
		if (Check_IsTargetableTile(mapData, mapInfo, CheckIndex) == false) {
			continue;
		}

		//Check target tile is reachable
		if (Check_IsReachableTile(mapData, mapInfo, originIndex, CheckIndex) == false) {
			continue;
		}

		pLinkableTiles->Add(CheckIndex);
		bFindLinkableTile = true;
	}

	return bFindLinkableTile;
}

bool AHHM_Manager_Navigation::Link_Tile(int32* pNavLinkIndex, const int32& originTileIndex, const int32& destTileIndex, const FHHM_MapInfo& mapInfo) {
	if (pNavLinkIndex == nullptr) {
		//Exception
		return false;
	}

	UWorld*	pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception
		return false;
	}

	//calculate how far they are vertically
	const int32 OriginIndex_Vertical = originTileIndex / mapInfo.MapSize_Horizontal;
	const int32 DestIndex_Vertical = destTileIndex / mapInfo.MapSize_Horizontal;
	const int32 HeightGap = (OriginIndex_Vertical - DestIndex_Vertical < 0) ? (OriginIndex_Vertical - DestIndex_Vertical) * -1 : OriginIndex_Vertical - DestIndex_Vertical;

	//Make some information for NavLinkProxy
	const FVector	Link_Offset = FVector(mapInfo.TileSize * 0.5f, 0.0f, mapInfo.TileSize);
	const FVector	Location_Origin = AHHM_Manager_Math_Grid::Convert_IndexToTranslation(mapInfo, originTileIndex);
	const FVector	Location_Destination = AHHM_Manager_Math_Grid::Convert_IndexToTranslation(mapInfo, destTileIndex);

	const FVector	LinkLocation_Left = Link_Offset;
	const FVector	LinkLocation_Right = (Location_Destination - Location_Origin) + Link_Offset;

	const FRotator	Rotator = FRotator::ZeroRotator;

	//Create NavLinkProxy and set basic inform
	AHHM_NavLinkProxy* pNavLinkProxy = nullptr;
	//switch (HeightGap) {
	//case 1:
	//	pNavLinkProxy = pWorld->SpawnActor<AHHM_NavLinkProxy_H1>(Location_Origin, Rotator);
	//	break;
	//case 2:
	//	pNavLinkProxy = pWorld->SpawnActor<AHHM_NavLinkProxy_H2>(Location_Origin, Rotator);
	//	break;
	//case 0:
	//	//Exception Why Height 0?
	//default:
	//	return false;
	//}
	pNavLinkProxy = pWorld->SpawnActor<AHHM_NavLinkProxy>(Location_Origin, Rotator);
	if (pNavLinkProxy == nullptr) {
		//Exception
		return false;
	}
	pNavLinkProxy->Set_LinkData(LinkLocation_Left, LinkLocation_Right);
	pNavLinkProxy->Set_Height(HeightGap);

	//Add Created NavLinkProxy to container and finish the function
	int32 NavLinkIndex = m_Arr_NavLink.Add(pNavLinkProxy);
	*pNavLinkIndex = NavLinkIndex;
	/*if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Create NavLink"));
	}*/
	return true;
}



#if true
void AHHM_Manager_Navigation::Refresh_Region(void) {
	UWorld* pWorld = nullptr;
	pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception
		return;
	}
	AGameModeBase* pGameMode = nullptr;
	pGameMode = pWorld->GetAuthGameMode();
	if (pGameMode == nullptr) {
		//Exception
		return;
	}
	AHHM_GameMode_LocalMap* pGameMode_LocalMap = nullptr;
	pGameMode_LocalMap = Cast<AHHM_GameMode_LocalMap>(pGameMode);
	if (pGameMode_LocalMap == nullptr) {
		//Exception
		return;
	}
	ALocalMap* pLocalMap = nullptr;
	pLocalMap = pGameMode_LocalMap->Get_LocalMap();
	if (pLocalMap == nullptr) {
		//Exception
		return;
	}

	const TArray<FHHM_TileData>&	LocalMapData = pLocalMap->Get_MapData();
	const int32						MapSize = LocalMapData.Num();
	
	//****	Clear Old Region and Initialize it	****//
	m_Arr_Region.Empty();
	m_Arr_Region.Init(-2, MapSize);

	//****	Find all bloced index	****//
	for (int32 CurIndex = 0; CurIndex < MapSize; ++CurIndex) {
		if (LocalMapData[CurIndex].IsPassable == false) {
			m_Arr_Region[CurIndex] = -1;
		}
	}

	//****	find a start point of region and fill it	****//
	int32					NextRegion = 0;
	const FHHM_MapInfo&		MapInfo = pGameMode_LocalMap->Get_MapInfo();
	for (int32 CurIndex = 0; CurIndex < MapSize; ++CurIndex) {
		if (m_Arr_Region[CurIndex] != -2) {
			continue;
		}

		m_Arr_Region[CurIndex] = NextRegion;
		++NextRegion;

		Fill_EmptyRegion(MapInfo,CurIndex);
	}
}

void AHHM_Manager_Navigation::Fill_EmptyRegion(const FHHM_MapInfo& MapInfo, int32 StartIndex) {
	const int32 CurRegion = m_Arr_Region[StartIndex];
	TArray<int32>	Arr_Fill_IndexQueue;
	Arr_Fill_IndexQueue.Add(StartIndex);
	
	while (Arr_Fill_IndexQueue.Num() > 0) {
		int32	Index_Horizontal = Arr_Fill_IndexQueue[0] % MapInfo.MapSize_Horizontal;
		int32	Index_Vertical = Arr_Fill_IndexQueue[0] / MapInfo.MapSize_Horizontal;

		//**** Check near index is notInitialized and if that index is not initialized, add to queue.	****//
			//****	Get near index for checking work. if near index isn't valid, set it as -1	****//
		int32	Index_Up = (Index_Vertical + 1 >= MapInfo.MapSize_Vertical) ? -1 : (Index_Vertical + 1) * MapInfo.MapSize_Horizontal + Index_Horizontal;
		int32	Index_Right = (Index_Horizontal + 1 >= MapInfo.MapSize_Horizontal) ? -1 : Index_Vertical * MapInfo.MapSize_Horizontal + Index_Horizontal + 1;
		int32	Index_Down = (Index_Vertical - 1 < 0) ? -1 : (Index_Vertical - 1) * MapInfo.MapSize_Horizontal + Index_Horizontal;
		int32	Index_Left = (Index_Horizontal - 1 < 0) ? -1 : Index_Vertical * MapInfo.MapSize_Horizontal + Index_Horizontal - 1;
		int32	Arr_Near[4] = { Index_Up, Index_Right, Index_Down, Index_Left };	//C++ Array Mark for just in case	Mark.HHM
		for (int32 i = 0; i < 4; ++i) {
			if (Arr_Near[i] == -1) continue;
			if (m_Arr_Region[Arr_Near[i]] != -2) continue;

			Arr_Fill_IndexQueue.Add(Arr_Near[i]);
		}

		//****	Fill Current Index and remove from queue	****//
		m_Arr_Region[Arr_Fill_IndexQueue[0]] = CurRegion;
		Arr_Fill_IndexQueue.RemoveAt(0);
	}
	
}
#endif	//Region