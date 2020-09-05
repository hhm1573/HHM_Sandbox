// Fill out your copyright notice in the Description page of Project Settings.

#include "LocalMap.h"

#include "Header/Macro.h"
#include "Header/Struct_Tile.h"
#include "Tile/Base/HHM_Tile.h"

#include "Base/Component/HHM_Component_LocalMapRender.h"

#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Base/GameMode/HHM_GameMode_LocalMap.h"
#include "Manager/Render/HHM_Manager_Render.h"

#include "Manager/Math/Grid/HHM_Manager_Math_Grid.h"
#include "Data/LocalMap/Generator/Generator_LocalMap.h"

#include "Components/BoxComponent.h"
#include "Components/InstancedStaticMeshComponent.h"

#include "Entity/HHM_Entity.h"

#include "Manager/Navigation/NavLink/HHM_NavLinkProxy.h"



const FVector Const_TileOffset = FVector(50.0f, 0.0f, 0.0f);



// Sets default values
ALocalMap::ALocalMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (m_ID_LocalMap != -1) {
		m_ID_LocalMap = -1;
	}



	

	/*if (m_pComp_LocalMapRender == nullptr) {
		m_pComp_LocalMapRender = CreateDefaultSubobject<UHHM_Component_LocalMapRender>(TEXT("LocalMapRenderComponent"));
	}*/
}

#pragma region Unreal Method

// Called when the game starts or when spawned
void ALocalMap::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ALocalMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALocalMap::BeginDestroy() {
	Super::BeginDestroy();
	//Delete all actor

	//HHM Note : Auto type
	for (auto pairElem : m_Container_Entity) {
		ALocalMap::Entity_Deregister(pairElem.Value);
	}

	//HHM Note : Log Entity container size.
}

#pragma endregion

#pragma region Initialize related

bool ALocalMap::Initialize_LocalMap(void){
	//Local Map should been validfied before BeginPlay, by LocalMap Manager right after spawned.
	if (m_ID_LocalMap < 0) {
		//Exception
		return false;
	}



	if (m_pManager_Tile == nullptr) {
		Request_TileManager();
		if (m_pManager_Tile == nullptr) {
			//Exception
			return false;
		}
	}



	const bool Succeed_Initialize_TouchPanel = ALocalMap::Initialize_TouchPanel();
	if (Succeed_Initialize_TouchPanel == false) {
		//Exception
		return false;
	}



	const bool Succeed_Initialize_MeshComponent = ALocalMap::Initialize_MeshComponent();
	if (Succeed_Initialize_MeshComponent == false) {
		//Exception
		return false;
	}

	const bool Succeed_Initialize_MeshContainer = ALocalMap::Initialize_Container_InstancedMesh();
	if (Succeed_Initialize_MeshContainer == false) {
		//Exception
		return false;
	}

	ALocalMap::Render_Reset();

	return true;
}

void ALocalMap::Validfy_LocalMap(int32 _id, int32 _index_Horizontal, int32 _index_Vertical,const FHHM_MapInfo& _mapInfo)
{
	if (m_ID_LocalMap != -1) {
		//Exception : Validfy twice
	}

	m_ID_LocalMap = _id;
	m_Index_Horizontal = _index_Horizontal;
	m_Index_Vertical = _index_Vertical;

	float Location_Horizontal = _index_Horizontal * HHM_OFFSET_LOCALMAP;
	float Location_Vertical = _index_Vertical * HHM_OFFSET_LOCALMAP;
	FVector2D Location_Offset = FVector2D(Location_Horizontal, Location_Vertical);

	m_Location_Offset = Location_Offset;



	m_MapInfo = _mapInfo;

	//Set MapData
	/*m_MapData.Container_TileData = _mapData.Container_TileData;
	m_MapData.Container_TileMovementData = _mapData.Container_TileMovementData;*/
	//HHM Note : MapData set by LocalMap Manager while it creates localmap. it gets mapdata's address from new local map and directly modify that.

	bool IsSucceed_InitializeLocalMap = ALocalMap::Initialize_LocalMap();
	if (IsSucceed_InitializeLocalMap == false) {
		//Exception
		return;
	}

	return;
}

bool ALocalMap::Initialize_TouchPanel(void) {
	if (m_pComponent_TouchPanel == nullptr) {
		m_pComponent_TouchPanel = NewObject<UBoxComponent>(this, TEXT("TouchPanel"));
		if (m_pComponent_TouchPanel == nullptr) {
			//Exception Error during initialize component
			return false;
		}

		if (m_pComponent_TouchPanel != nullptr) {
			m_pComponent_TouchPanel->SetupAttachment(RootComponent);
		}
	}

	if (m_ID_LocalMap < 0 || m_MapInfo.MapSize_Horizontal <= 0 || m_MapInfo.MapSize_Vertical <= 0) {
		//Exception LocalMap not validfied
		return false;
	}



	float	Size_Panel_Horizontal = m_MapInfo.MapSize_Horizontal * HHM_TILE_SIZE;
	float	Size_Panel_Vertical = m_MapInfo.MapSize_Vertical * HHM_TILE_SIZE;
	FVector Size_Panel = FVector(Size_Panel_Horizontal, 0.0f, Size_Panel_Vertical);

	m_pComponent_TouchPanel->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	m_pComponent_TouchPanel->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	m_pComponent_TouchPanel->SetBoxExtent(Size_Panel * 0.5f);
	m_pComponent_TouchPanel->SetRelativeLocation(Size_Panel * 0.5f);

	return true;
}



void ALocalMap::Clear_Map(void) {
	//HHM Note : Update this function after reworking MapData Structure
	m_MapData.Container_TileData.Empty();
	m_MapData.Container_TileMovementData.Empty();
}

void ALocalMap::Refresh_MovementData(void) {
	const int32 Num_TileContainer = m_MapData.Container_TileData.Num();
	const int32 Num_MovementDataContainer = m_MapData.Container_TileMovementData.Num();

	if (Num_TileContainer != Num_MovementDataContainer) {
		m_MapData.Container_TileMovementData.Empty();
		m_MapData.Container_TileMovementData.Init(FHHM_TileMovementInfo(), Num_TileContainer);
	}

	for (int32 index_Tile = 0; index_Tile < Num_TileContainer; ++index_Tile) {
		//m_Arr_MovementData[index_Tile].Passable = !m_Arr_MapData[index_Tile].IsBlockPassage;
		ALocalMap::Translate_TileInfo_To_MovementData(m_MapData.Container_TileData[index_Tile], m_MapData.Container_TileMovementData[index_Tile]);
	}
}

void ALocalMap::Request_TileManager(void) {
	UWorld* pWorld = nullptr;
	pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception
		return;
	}

	AGameModeBase* pGameMode_Raw = nullptr;
	pGameMode_Raw = pWorld->GetAuthGameMode();
	if (pGameMode_Raw == nullptr) {
		//Exception
		return;
	}

	AHHM_GameMode_LocalMap* pGameMode = nullptr;
	pGameMode = Cast<AHHM_GameMode_LocalMap>(pGameMode_Raw);
	if (pGameMode == nullptr) {
		//Exception
		return;
	}

	m_pManager_Tile = pGameMode->Get_Manager_Tile();

}



bool ALocalMap::Clear_MeshComponent(void)
{
	for (auto& pair : m_Container_Comp_InstancedMesh) {
		for (UInstancedStaticMeshComponent* pInstancedMesh : pair.Value.Arr_pInstancedStaticMesh) {
			if (pInstancedMesh != nullptr) {
				pInstancedMesh->ClearInstances();
			}
		}
	}

	return true;
}

bool ALocalMap::Clear_RenderData(void)
{
	m_Container_RenderData.Empty();
	return true;
}

#pragma endregion

#pragma region Interact with map

const FHHM_TileData& ALocalMap::Get_TileInfo(int32 index_Horizontal, int32 index_Vertical) const {
	int32 Index = AHHM_Manager_Math_Grid::Index_Combine(index_Horizontal, index_Vertical, m_MapInfo);
	if (Index < 0 || Index >= m_MapData.Container_TileData.Num()) {
		//Exception
		//HHM Note : Return Dummy or air tile might be better
		return m_MapData.Container_TileData[0];
	}

	return m_MapData.Container_TileData[Index];
}

void ALocalMap::Set_Tile_At_Pos(int32 _index_Horizontal, int32 _index_Vertical, const FHHM_TileData& _tileData) {
	//Get Info from LocalMap Manager.
	//Set Tile is called by debug or cheat function
	if (Check_IsValidPos(_index_Horizontal, _index_Vertical) == false) {
		//Exception
		return;
	}

	const int32 Index_Input = _index_Vertical * m_MapInfo.MapSize_Horizontal + _index_Horizontal;
	m_MapData.Container_TileData[Index_Input] = _tileData;

	//Save Location data
	m_MapData.Container_TileData[Index_Input].Index_Horizontal = _index_Horizontal;
	m_MapData.Container_TileData[Index_Input].Index_Vertical = _index_Vertical;



	//Tile 에 타일인포 받아오는거 만들어서 체력같은거 설정된 값으로 가져올수있게끔 만들자

	ALocalMap::Update_TileRenderData(m_MapData.Container_TileData[Index_Input]);
}

void ALocalMap::Set_Tile_At_Pos(int32 _index_Horizontal, int32 _index_Vertical, int32 _tile_ID)
{
	if (m_pManager_Tile == nullptr) {
		//Exception
		return;
	}

	const FHHM_TileData& TileData = m_pManager_Tile->Get_DefaultTileInfo_ByID(_tile_ID);

	ALocalMap::Set_Tile_At_Pos(_index_Horizontal, _index_Vertical, TileData);
}



bool ALocalMap::Place_Tile(int32 _index_Horizontal, int32 _index_Vertical, AEntity* _pPlacer, FHHM_TileData _tileData)
{
	const bool isValidIndex = Check_IsValidPos(_index_Horizontal, _index_Vertical);
	if (isValidIndex == false) {
		//Exception
		return false;
	}

	const bool isLocationPlaceable = Check_Location_TilePlaceable(_index_Horizontal, _index_Vertical);
	if (isLocationPlaceable == false) {
		return false;
	}

	// This is actual tile that will be placed
	FHHM_TileData TileData_Place = _tileData.Tile->On_Placed(this, _tileData, _pPlacer, m_MapInfo);

	ALocalMap::Set_Tile_At_Pos(_index_Horizontal, _index_Vertical, TileData_Place);

	return true;
}

bool ALocalMap::Place_Tile(int32 _index_Horizontal, int32 _index_Vertical, AEntity* _pPlacer, int32 _tile_ID)
{
	const bool IsValidIndex = Check_IsValidPos(_index_Horizontal, _index_Vertical);
	if (IsValidIndex == false) {
		//Exception
		return false;
	}

	const bool IsLocationPlaceable = Check_Location_TilePlaceable(_index_Horizontal, _index_Vertical);
	if (IsLocationPlaceable == false) {
		return false;
	}

	if (m_pManager_Tile == nullptr) {
		ALocalMap::Request_TileManager();
		if (m_pManager_Tile == nullptr) {
			//Exception
			return false;
		}
	}

	FHHM_TileData TileData_Default = m_pManager_Tile->Get_DefaultTileInfo_ByID(_tile_ID);

	FHHM_TileData TileData_Place = TileData_Default.Tile->On_Placed(this, TileData_Default, _pPlacer, m_MapInfo);

	ALocalMap::Set_Tile_At_Pos(_index_Horizontal, _index_Vertical, TileData_Place);

	return true;
}

//void ALocalMap::Set_Tile_At_Pos_Temp(int32 _index_Horizontal, int32 _index_Vertical, int32 _tileID) {
//
//	UHHM_GameInstance* pGameInstance = nullptr;
//	pGameInstance = Cast<UHHM_GameInstance>(GetGameInstance());
//	if (pGameInstance == nullptr) {
//		//Exception
//		return;
//	}
//
//	AHHM_Manager_Tile* pManager_Tile = nullptr;
//	pManager_Tile = pGameInstance->Get_Manager_Tile();
//	if (pManager_Tile == nullptr) {
//		//Exception
//		return;
//	}
//	const FHHM_TileData& TileInfo = pManager_Tile->Get_DefaultTileInfo_ByID(_tileID);
//
//	//Set Tile Info
//	Set_Tile_At_Pos(_index_Horizontal, _index_Vertical, TileInfo);
//
//	//Update Render Info
//
//	AHHM_Manager_Render* pManager_Render = nullptr;
//	pManager_Render = pGameInstance->Get_Manager_Render();
//	if (pManager_Render == nullptr) {
//		//Exception
//		return;
//	}
//
//	pManager_Render->Set_TileRenderData(AHHM_Manager_Math_Grid::Index_Combine(_index_Horizontal, _index_Vertical, m_MapInfo), TileInfo.ID, 0, FTransform());
//}

bool ALocalMap::Damage_Tile(int32 damage, EHHM_DamageType damage_Type, class APawn* attack_Pawn, int32 index_Horizontal, int32 index_Vertical) {
	//Check is input index value is valid
	if (Check_IsValidPos(index_Horizontal, index_Vertical) == false) {
		//Exception
		return false;
	}
	const int32 index_Tile = AHHM_Manager_Math_Grid::Index_Combine(index_Horizontal, index_Vertical, m_MapInfo);
	
	//Check if damaged tile is Air tile. if it is, do exception work.
	if (m_MapData.Container_TileData[index_Tile].ID == 0) {
		//Exception
		return false;
	}
	
	//Call tile's On_Damaged function and get "after HP"
	const int32 TileHP_Before = m_MapData.Container_TileData[index_Tile].HP;
	int32 TileHP_After = m_MapData.Container_TileData[index_Tile].Tile->On_Damaged(this, m_MapData.Container_TileData[index_Tile], damage, damage_Type, attack_Pawn, m_MapInfo);

	//if "after HP" is lower than zero, call tile's destruction function and get after tile info and replace it
	if (TileHP_After <= 0) {
		//Call Damaged Tile's Destruct function and get result tile info
		FHHM_TileData	TileInfo_After = m_MapData.Container_TileData[index_Tile].Tile->On_Destruct(this, m_MapData.Container_TileData[index_Tile], m_MapInfo);

		//Set location information of result tile info
		TileInfo_After.Index_Horizontal = index_Horizontal;
		TileInfo_After.Index_Vertical = index_Vertical;

		//Store result tile info at map data
		////m_Arr_MapData[index_Tile] = TileInfo_After;
		ALocalMap::Set_Tile_At_Pos(index_Horizontal, index_Vertical, TileInfo_After);

		//////Temporary render info setting
		//////Cast<UHHM_GameInstance>(GetGameInstance())->Get_Manager_Render()->Set_TileRenderData(index_Tile, TileInfo_After.ID);
		//need link work
		//Cast<UHHM_GameInstance>(GetGameInstance())->Get_Manager_Render()->Update_RenderInfo_Tile(this, TileInfo_After);

		return true;
	}

	m_MapData.Container_TileData[index_Tile].HP = TileHP_After;
	return false;
}



#pragma endregion



#pragma region Location Check

bool ALocalMap::IsLocationStandable(int32 _index) const {
	const bool isLocationPassable = ALocalMap::IsLocationPassable(_index);
	if (isLocationPassable == false) {
		return false;
	}

	const bool isLocationOnGround = ALocalMap::IsOnGround(_index);
	if (isLocationOnGround == false) {
		return false;
	}

	return true;
}

bool ALocalMap::IsLocationPassable(int32 _index) const {
	const bool isValidIndex = ALocalMap::Check_IsValidIndex(_index);
	if (isValidIndex == false) {
		//Exception
		return false;
	}

	return m_MapData.Container_TileData[_index].IsPassable;
}

bool ALocalMap::IsOnGround(int32 _index_Horizontal, int32 _index_Vertical) const {
	const bool isValidLocation = ALocalMap::Check_IsValidPos(_index_Horizontal, _index_Vertical);
	if (isValidLocation == false) {
		return false;
	}

	if (_index_Vertical == 0) {
		return false;
	}

	const int32 Index_Below = AHHM_Manager_Math_Grid::Index_Combine(_index_Horizontal, _index_Vertical - 1, m_MapInfo);
	
	return !m_MapData.Container_TileData[Index_Below].IsPassable;
}

bool ALocalMap::IsOnGround(int32 _index) const {
	const bool isValidIndex = ALocalMap::Check_IsValidIndex(_index);
	if (isValidIndex == false) {
		return false;
	}

	if (_index < m_MapInfo.MapSize_Horizontal) {
		return false;
	}

	const int32 Index_Below = _index - m_MapInfo.MapSize_Horizontal;

	return !m_MapData.Container_TileData[Index_Below].IsPassable;
}

bool ALocalMap::IsAtCeiling(int32 _index_Horizontal, int32 _index_Vertical, int8 _entity_Height) const {
	
	const bool isValidLocation = ALocalMap::Check_IsValidPos(_index_Horizontal, _index_Vertical);
	if (isValidLocation == false) {
		//Exception
		return false;
	}

	int32 Index_Above = _index_Vertical + _entity_Height;	//if entity is standing on y:1 and it's height is 2 tile high, entity's head will be at y:2 and above tile will be y:3
	
	if (Index_Above >= m_MapInfo.MapSize_Vertical) {
		return true;
	}

	int32 TileIndex_Above = AHHM_Manager_Math_Grid::Index_Combine(_index_Horizontal, _index_Vertical, m_MapInfo);

	if (TileIndex_Above < 0 || TileIndex_Above >= m_MapData.Container_TileData.Num()) {
		//Exception
		return false;
	}

	return !m_MapData.Container_TileData[TileIndex_Above].IsPassable;

}

bool ALocalMap::IsAtCeiling(int32 _index, int8 _entity_Height) const {
	
	const int32 Num_Map = m_MapData.Container_TileData.Num();

	const bool isValidIndex = ALocalMap::Check_IsValidIndex(_index);
	if (isValidIndex == false) {
		//Exception
		return false;
	}

	const int32 Index_Above = _index + (m_MapInfo.MapSize_Horizontal * _entity_Height);

	if (Index_Above >= Num_Map) {
		return true;
	}

	return !m_MapData.Container_TileData[Index_Above].IsPassable;

}



bool ALocalMap::Check_Location_TilePlaceable(int32 _index_Horizontal, int32 _index_Vertical) {
	const bool IsValidIndex = Check_IsValidPos(_index_Horizontal, _index_Vertical);
	if (IsValidIndex == false) {
		//Exception
		return false;
	}

	const int32 Index_Combine = AHHM_Manager_Math_Grid::Index_Combine(_index_Horizontal, _index_Vertical, m_MapInfo);

	return m_MapData.Container_TileData[Index_Combine].ID == 0 ? true : false;
}



bool ALocalMap::IsAreaPassable(int32 _index, int8 _entity_Width, int8 _entity_Height) const {
	TArray<int32>	Container_Index_NeedCheck_IsPassable;

	//-------- Area Valid check --------//
	FVector2D	Location_LeftBottom = FVector2D(-1.0f, -1.0f);
	if (AHHM_Manager_Math_Grid::Index_Seperate(Location_LeftBottom, _index, m_MapInfo) == false) {
		//Exception
		return false;
	}

	FVector2D	Location_RightTop = Location_LeftBottom + FVector2D(float(_entity_Width - 1), float(_entity_Height - 1));

	if (Location_LeftBottom.X < 0 || Location_LeftBottom.X >= m_MapInfo.MapSize_Horizontal
		|| Location_LeftBottom.Y < 0 || Location_LeftBottom.Y >= m_MapInfo.MapSize_Vertical) {	//Honestly, this check is not an essential. because this location already confirm as valid on "Index_Seperate" function. but i think it looks good to leave it
		//Exception warning
		return false;
	}

	if (Location_RightTop.X < 0 || Location_RightTop.X >= m_MapInfo.MapSize_Horizontal
		|| Location_RightTop.Y < 0 || Location_RightTop.Y >= m_MapInfo.MapSize_Vertical) {
		return false;
	}



	//-------- Ground check --------//
	const bool isStandable_LeftBottom = ALocalMap::IsLocationStandable(_index);
	if (isStandable_LeftBottom == false) {
		return false;
	}

	for (int32 index = 1; index < _entity_Height; ++index) {
		FVector2D	Location_Above = Location_LeftBottom + FVector2D(0.0f, index);
		int32		Index_Above = AHHM_Manager_Math_Grid::Index_Combine(Location_Above.X, Location_Above.Y, m_MapInfo);
		Container_Index_NeedCheck_IsPassable.Add(Index_Above);
	}



	FVector2D	Location_Check_Before = Location_LeftBottom;
	for (int32 index_Horizontal = 1; index_Horizontal < _entity_Width; ++index_Horizontal) {

		bool	isFound_StandableLoc = false;

		for (int32 index_Vertical = 1; index_Vertical >= -1; --index_Vertical) {
			FVector2D	Location_Check = Location_Check_Before + FVector2D(1.0f, index_Vertical);
			int32		Index_Check = AHHM_Manager_Math_Grid::Index_Combine(Location_Check.X, Location_Check.Y, m_MapInfo);
			const bool	isStandable_Check = ALocalMap::IsLocationStandable(Index_Check);

			if (isStandable_Check == true) {

				isFound_StandableLoc = true;

				int32		Index_Top_Check = Location_Check.Y + (_entity_Height - 1);
				const bool	IsIndexTopTooLow = Index_Top_Check < Location_RightTop.Y ? true : false;
				int32		Index_Top_ToAdd = Index_Top_Check;
				if (IsIndexTopTooLow == true) {
					Index_Top_ToAdd = Location_RightTop.Y;
				}

				for (int32 index_Vertical_ToAddQueue = 1; index_Vertical_ToAddQueue <= Index_Top_ToAdd; ++index_Vertical_ToAddQueue) {
					FVector2D		Location_ToAddQueue = Location_Check + FVector2D(0.0f, float(index_Vertical_ToAddQueue));
					int32			Index_ToAddQueue = AHHM_Manager_Math_Grid::Index_Combine(Location_ToAddQueue.X, Location_ToAddQueue.Y, m_MapInfo);
					Container_Index_NeedCheck_IsPassable.Add(Index_ToAddQueue);
				}

				Location_Check_Before = Location_Check;

				break;
			}
		}

		if (isFound_StandableLoc == false) {
			return false;
		}
	}



	//-------- Air Check --------//
	const int32 Size_PassableCheckQueue = Container_Index_NeedCheck_IsPassable.Num();
	for (int32 index = 0; index < Size_PassableCheckQueue; ++index) {
		const bool isIndexPassable = ALocalMap::IsLocationPassable(Container_Index_NeedCheck_IsPassable[index]);
		if (isIndexPassable == false) {
			return false;
		}
	}



	return true;
}

bool ALocalMap::IsAreaStandable(int32 _index, int8 _entity_Width, int8 _entity_Height) const {
	TArray<int32>	Container_Index_NeedCheck_IsPassable;

	//-------- Area Valid check --------//
	FVector2D	Location_LeftBottom = FVector2D(-1.0f, -1.0f);
	if (AHHM_Manager_Math_Grid::Index_Seperate(Location_LeftBottom, _index, m_MapInfo) == false) {
		//Exception
		return false;
	}

	FVector2D	Location_RightTop = Location_LeftBottom + FVector2D(float(_entity_Width - 1), float(_entity_Height - 1));

	if (Location_LeftBottom.X < 0 || Location_LeftBottom.X >= m_MapInfo.MapSize_Horizontal
		|| Location_LeftBottom.Y < 0 || Location_LeftBottom.Y >= m_MapInfo.MapSize_Vertical) {	//Honestly, this check is not an essential. because this location already confirm as valid on "Index_Seperate" function. but i think it looks good to leave it
		//Exception warning
		return false;
	}

	if (Location_RightTop.X < 0 || Location_RightTop.X >= m_MapInfo.MapSize_Horizontal
		|| Location_RightTop.Y < 0 || Location_RightTop.Y >= m_MapInfo.MapSize_Vertical) {
		return false;
	}



	//-------- Ground check --------//
	const bool isStandable_LeftBottom = ALocalMap::IsLocationStandable(_index);
	if (isStandable_LeftBottom == false) {
		return false;
	}

	for (int32 index = 1; index < _entity_Height; ++index) {
		FVector2D	Location_Above = Location_LeftBottom + FVector2D(0.0f, index);
		int32		Index_Above = AHHM_Manager_Math_Grid::Index_Combine(Location_Above.X, Location_Above.Y, m_MapInfo);
		Container_Index_NeedCheck_IsPassable.Add(Index_Above);
	}



	FVector2D	Location_Check_Before = Location_LeftBottom;
	int32		HeightGap = 0;
	for (int32 index_Horizontal = 1; index_Horizontal < _entity_Width; ++index_Horizontal) {

		bool	isFound_StandableLoc = false;

		for (int32 index_Vertical = 1; index_Vertical >= -1; --index_Vertical) {
			FVector2D	Location_Check = Location_Check_Before + FVector2D(1.0f, index_Vertical);
			int32		Index_Check = AHHM_Manager_Math_Grid::Index_Combine(Location_Check.X, Location_Check.Y, m_MapInfo);
			const bool	isStandable_Check = ALocalMap::IsLocationStandable(Index_Check);

			if (isStandable_Check == true) {
				bool isGroundSmooth = HeightGap * index_Vertical < 0 ? false : true;
				if (isGroundSmooth == false) {
					return false;
				}

				isFound_StandableLoc = true;

				int32		Index_Top_Check = Location_Check.Y + (_entity_Height - 1);
				const bool	IsIndexTopTooLow = Index_Top_Check < Location_RightTop.Y ? true : false;
				int32		Index_Top_ToAdd = Index_Top_Check;
				if (IsIndexTopTooLow == true) {
					Index_Top_ToAdd = Location_RightTop.Y;
				}

				for (int32 index_Vertical_ToAddQueue = 1; index_Vertical_ToAddQueue <= Index_Top_ToAdd; ++index_Vertical_ToAddQueue) {
					FVector2D		Location_ToAddQueue = Location_Check + FVector2D(0.0f, float(index_Vertical_ToAddQueue));
					int32			Index_ToAddQueue = AHHM_Manager_Math_Grid::Index_Combine(Location_ToAddQueue.X, Location_ToAddQueue.Y, m_MapInfo);
					Container_Index_NeedCheck_IsPassable.Add(Index_ToAddQueue);
				}

				Location_Check_Before = Location_Check;

				break;
			}
		}

		if (isFound_StandableLoc == false) {
			return false;
		}
	}



	//-------- Air Check --------//
	const int32 Size_PassableCheckQueue = Container_Index_NeedCheck_IsPassable.Num();
	for (int32 index = 0; index < Size_PassableCheckQueue; ++index) {
		const bool isIndexPassable = ALocalMap::IsLocationPassable(Container_Index_NeedCheck_IsPassable[index]);
		if (isIndexPassable == false) {
			return false;
		}
	}



	return true;
}

#pragma endregion




bool ALocalMap::Check_IsValidPos(int32 Index_Horizontal, int32 Index_Vertical) const {
	if (Index_Horizontal < 0 || Index_Vertical < 0)
		return false;

	if (Index_Horizontal >= m_MapInfo.MapSize_Horizontal || Index_Vertical >= m_MapInfo.MapSize_Vertical)
		return false;

	return true;
}

bool ALocalMap::Check_IsValidIndex(int32 _index) const {
	int32 Size_Map = m_MapData.Container_TileData.Num();

	if (_index < 0 || _index >= Size_Map) {
		return false;
	}

	return true;
}

void ALocalMap::Translate_TileInfo_To_MovementData(const FHHM_TileData& tileInfo, FHHM_TileMovementInfo& tileMovementInfo) {
	tileMovementInfo.Passable = tileInfo.IsPassable;
}



#pragma region Rendering

bool ALocalMap::Initialize_MeshComponent(void)
{
	m_pStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
	if (m_pStaticMesh == nullptr) {
		//Exception
		return false;
	}

	return true;
}

bool ALocalMap::Initialize_Container_InstancedMesh(void)
{
	//-------- Check Required Component --------//
	if (m_pStaticMesh == nullptr) {
		//Exception
		return false;
	}

	//-------- Get Tile Manager to get Tile container ---------//
	if (m_pManager_Tile == nullptr) {
		ALocalMap::Request_TileManager();
		if (m_pManager_Tile == nullptr) {
			//Exception
			return false;
		}
	}

	TMap<int32, AHHM_Tile*>& Ref_TileContainer = m_pManager_Tile->Get_TileArr_Ref();



	//iterate container and if tile's render type is 'instanced', create render info based on tile's info
	const int32 Num_TileContainer = Ref_TileContainer.Num();

	for (int32 index_Tile = 0; index_Tile < Num_TileContainer; ++index_Tile) {
		if (Ref_TileContainer[index_Tile] == nullptr) {
			//Exception
			return false;
		}

		//Get Tile's render info data
		const FHHM_RenderInfo& TileRenderInfo = Ref_TileContainer[index_Tile]->Get_RenderInfo();
		if (TileRenderInfo.eRenderType != EHHM_RenderType::RType_Instanced) {
			continue;
		}

		//Check if tile is already registered
		bool IsTileAlreadyRegistered = m_Container_Comp_InstancedMesh.Contains(Ref_TileContainer[index_Tile]->Get_TileID());
		if (IsTileAlreadyRegistered == true) {
			//Exception
			return false;
		}



		//make some place on map for tile
		m_Container_Comp_InstancedMesh.Add(index_Tile, FHHM_InstancedMeshArray());

		//Filling render manager's instance map based on tile's material
		int32 Num_TileMaterial = TileRenderInfo.Num_Material;
		for (int32 index_Material = 0; index_Material < Num_TileMaterial; ++index_Material) {

			//Check tile's material is already registered
			int32 Num_Registered_Material = m_Container_Comp_InstancedMesh[index_Tile].Arr_pInstancedStaticMesh.Num();
			if (index_Material < Num_Registered_Material) {
				//Exception;
				continue;
			}

			//Check is material is availiable
			if (TileRenderInfo.Arr_Material[index_Material] == nullptr) {
				//Exception
				return false;
			}

			//Create Mesh Component than set mesh and material
			FString Name_InstancedMesh = FString::Printf(TEXT("InstancedMesh_%d_%d"), index_Tile, index_Material);
			UInstancedStaticMeshComponent* pInstancedStaticMeshComponent = nullptr;
			//pInstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(*Name_InstancedMesh);
			pInstancedStaticMeshComponent = NewObject<UInstancedStaticMeshComponent>(this, *Name_InstancedMesh);
			pInstancedStaticMeshComponent->SetupAttachment(RootComponent);
			pInstancedStaticMeshComponent->SetStaticMesh(m_pStaticMesh);
			pInstancedStaticMeshComponent->SetMaterial(0, TileRenderInfo.Arr_Material[index_Material]);

			//HHM Note : Tile Size Scaling Part.
			float Scale_InstancedStaticMesh = HHM_TILE_SIZE / HHM_TILE_MESH_SIZE;
			pInstancedStaticMeshComponent->SetWorldScale3D(FVector(Scale_InstancedStaticMesh));

			//Collision Setting
			const bool IsPassable = Ref_TileContainer[index_Tile]->Get_IsPassable();
			if (IsPassable) {
				pInstancedStaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
			}
			else {
				pInstancedStaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
			}

			//Rendering Setting
			const bool IsNeedToRender = TileRenderInfo.IsNeedToRender;
			if (IsNeedToRender == false) {
				pInstancedStaticMeshComponent->bHiddenInGame = true;
			}

			//Register instanced static mesh component on renderer's map
			m_Container_Comp_InstancedMesh[index_Tile].Arr_pInstancedStaticMesh.Add(pInstancedStaticMeshComponent);
			m_Container_Comp_InstancedMesh[index_Tile].Arr_pInstancedStaticMesh[index_Material]->RegisterComponent();
			//m_Container_Comp_InstancedMesh[index_Tile].Arr_pInstancedStaticMesh[index_Material]->RegisterComponent();
		}
	}

	m_pComp_InstancedStaticMesh_Debug = m_Container_Comp_InstancedMesh[1].Arr_pInstancedStaticMesh[0];

	return true;
}



bool ALocalMap::Render_Reset()
{
	m_MaxMapIndex = (m_MapInfo.MapSize_Horizontal * m_MapInfo.MapSize_Vertical) - 1;

	int32	ArrSize_Input = m_MapData.Container_TileData.Num();

	//Clear RenderData Array and Copy data from Input TileInfo Data
	Clear_RenderData();
	for (int32 CurArrIndex = 0; CurArrIndex < ArrSize_Input; ++CurArrIndex) {
		FHHM_RenderData		TileRenderData = FHHM_RenderData();
		TileRenderData.Tile_ID = m_MapData.Container_TileData[CurArrIndex].ID;
		TileRenderData.Tile_SubID = m_MapData.Container_TileData[CurArrIndex].SubID;
		m_Container_RenderData.Add(TileRenderData);
	}

	//Check is RenderArray's size is same as InputArray's size. just in case
	int32	ArrSize_Render = m_Container_RenderData.Num();
	if (ArrSize_Input != ArrSize_Render) {
		//Exception
		return false;
	}


	//Clear Instances of Mesh and fill with new Render data
	Clear_MeshComponent();
	for (int32 CurArrIndex = 0; CurArrIndex < ArrSize_Render; ++CurArrIndex) {
		if (Check_ValidRenderID(m_Container_RenderData[CurArrIndex].Tile_ID, m_Container_RenderData[CurArrIndex].Tile_SubID) == false) {
			//Exception
			return false;
		}

		FTransform Transform = FTransform();
		RenderInstance_Add(m_Container_RenderData[CurArrIndex].Tile_ID, m_Container_RenderData[CurArrIndex].Tile_SubID, CurArrIndex, Transform);

		if (m_Container_RenderData[CurArrIndex].Render_Index < 0) {
			//Exception
			return false;
		}
	}

	UpdateComponentTransforms();

	return true;
}



bool ALocalMap::Set_TileRenderData(int32 _index, int32 _tileID, int32 _tileSubID, FTransform& _transform)
{
	const bool isValidIndex = Check_ValidIndex(_index);
	if (isValidIndex == false) {
		//Exception
		return false;
	}

	const bool isValidTileID = Check_ValidRenderID(_tileID, _tileSubID);
	if (isValidTileID == false) {
		//Exception
		return false;
	}



	const bool Succeed_RemoveInstance = RenderInstance_Remove(_index);
	if (Succeed_RemoveInstance == false) {
		//Exception
		return false;
	}

	const bool Succeed_AddInstance = RenderInstance_Add(_tileID, _tileSubID, _index, _transform);
	if (Succeed_AddInstance == false) {
		//Exception
		return false;
	}

	return true;
}

bool ALocalMap::Update_TileRenderData(const FHHM_TileData& _tileData)
{
	//null check
	if (_tileData.Tile == nullptr) {
		//Exception
		return false;
	}

	//Get essential information to update render
	FTransform	Transform_Local = FTransform();
	int32		index_instance = -1;

	//Call Tile's update_Render function		to get essential info(tranform_local, index_instance)
	const bool bIsUpdateSuccess = _tileData.Tile->Update_Render(index_instance, Transform_Local, this, _tileData);

	//if render info updating job is needed, do update
	if (bIsUpdateSuccess == true) {
		const int32 index_Update = AHHM_Manager_Math_Grid::Index_Combine(_tileData.Index_Horizontal, _tileData.Index_Vertical, m_MapInfo);

		const bool isRemoveSucceed = RenderInstance_Remove(index_Update);
		if (isRemoveSucceed == false) {
			//Exception
			return false;
		}

		const bool isAddSucceed = RenderInstance_Add(_tileData.ID, index_instance, index_Update, Transform_Local);
		if (isAddSucceed == false) {
			//Exception
			return false;
		}

		return true;
	}

	return false;
}



bool ALocalMap::RenderInstance_Add(int32 _tileID, int32 _tileSubID, int32 _index_Tile, FTransform& _tileTransform)
{
	const bool isValidTileID = Check_ValidRenderID(_tileID, _tileSubID);
	if (isValidTileID == false) {
		//Exception
		return false;
	}

	const bool isValidTileIndex = Check_ValidIndex(_index_Tile);
	if (isValidTileIndex == false) {
		//Exception
		return false;
	}

	//Adjusting target tile's translation
	FTransform	Transform_Adjusted = _tileTransform;
	FVector		Translation_Raw = FVector::ZeroVector;
	bool		IsSucceed_Calculate_Translation_Raw = AHHM_Manager_Math_Grid::Convert_Index_To_Translation(Translation_Raw, _index_Tile, m_MapInfo);
	if (IsSucceed_Calculate_Translation_Raw == false) {
		//Exception
		return false;
	}
	FVector		Translation = Translation_Raw + Const_TileOffset;
	Transform_Adjusted.SetTranslation(Translation);

	//Add instance and save index to renderinfo
	const int32 index_Instance = m_Container_Comp_InstancedMesh[_tileID].Arr_pInstancedStaticMesh[_tileSubID]->AddInstance(Transform_Adjusted);
	m_Container_RenderData[_index_Tile].Render_Index = index_Instance;
	m_Container_RenderData[_index_Tile].Tile_ID = _tileID;
	m_Container_RenderData[_index_Tile].Tile_SubID = _tileSubID;

	return true;
}

bool ALocalMap::RenderInstance_Remove(int32 _index_Tile)
{
	//Check input value is valid
	const bool isValidTileIndex = Check_ValidIndex(_index_Tile);
	if (isValidTileIndex == false) {
		//Exception
		return false;
	}

	//Check if target index don't have any render info
	if (m_Container_RenderData[_index_Tile].Render_Index < 0) {
		return false;
	}

	const int32 TileID_Target = m_Container_RenderData[_index_Tile].Tile_ID;
	const int32 TileSubID_Target = m_Container_RenderData[_index_Tile].Tile_SubID;

	//Check renderinfo's tileID is valid
	const bool isValidTileID = Check_ValidRenderID(TileID_Target, TileSubID_Target);
	if (isValidTileID == false) {
		//Exception
		return false;
	}



	//Prepare some variables for checking work
	const int32 Num_Instance_TargetMeshComp = m_Container_Comp_InstancedMesh[TileID_Target].Arr_pInstancedStaticMesh[TileSubID_Target]->GetNumRenderInstances();
	const int32 index_Instance_Last = Num_Instance_TargetMeshComp - 1;
	const int32 index_Instance_Target = m_Container_RenderData[_index_Tile].Render_Index;

	//Check if target(removing instance) is the last instance of target instancedMeshComp. if it isn't the last one, copy the last instance to target's info.
	if (index_Instance_Target != index_Instance_Last) {
		//Get the last instance's transform
		FTransform Transform_LastInstance = FTransform();
		if (m_Container_Comp_InstancedMesh[TileID_Target].Arr_pInstancedStaticMesh[TileSubID_Target]->GetInstanceTransform(index_Instance_Last, Transform_LastInstance) == false) {
			//Exception - cant get transform of last instance
		}

		//Find last instance's index
		FVector	Translation_LastInstance = Transform_LastInstance.GetTranslation();
		int32	index_LastInstance = -1;
		bool IsSucceed_Calculate_Index_LastInstance = AHHM_Manager_Math_Grid::Convert_Translation_To_Index(index_LastInstance, Translation_LastInstance, m_MapInfo);
		if (IsSucceed_Calculate_Index_LastInstance == false) {
			//Exception
			return false;
		}

		//Paste last instance's transform data to target's instance
		m_Container_Comp_InstancedMesh[TileID_Target].Arr_pInstancedStaticMesh[TileSubID_Target]->UpdateInstanceTransform(index_Instance_Target, Transform_LastInstance);
		//Update renderinfo of last instance's index
		m_Container_RenderData[index_LastInstance].Render_Index = index_Instance_Target;
	}

	//Delete last instance of target instanceMeshComp
	m_Container_Comp_InstancedMesh[TileID_Target].Arr_pInstancedStaticMesh[TileSubID_Target]->RemoveInstance(index_Instance_Last);

	return true;
}



bool ALocalMap::Check_ValidRenderID(int32 _tileID, int32 _tileSubID)
{
	if (_tileID < 0)	return false;
	if (_tileSubID < 0) return false;

	const bool isIDRegistered = m_Container_Comp_InstancedMesh.Contains(_tileID);
	if (isIDRegistered == false) {
		return false;
	}

	const int32 Num_SubID = m_Container_Comp_InstancedMesh[_tileID].Arr_pInstancedStaticMesh.Num();
	if (_tileSubID >= Num_SubID) {
		return false;
	}

	return true;
}

bool ALocalMap::Check_ValidIndex(int32 _index)
{
	if (_index < 0) return false;

	if (_index > m_MaxMapIndex) return false;

	return true;
}

#pragma endregion

#pragma region Entity

void ALocalMap::Container_Expand_AvailiableIndex_Entity(void) {

	int32 Num_Container_Entity = m_Container_Entity.Num();

	if (Num_Container_Entity > m_Size_Container_Entity) {
		//Exception Total Entity number is larger than expected. this exception check might already executed on Container_Resize function.
		//if you can't see resize functions error note, consider use resize function instead of this function since resize function can temporarily fix this issue.
	}

	if (m_Size_Container_Entity == 0) {
		
		m_Container_AvailiableIndex_Entity.Empty();
		if (Num_Container_Entity != 0) {
			//Exception there should be no entity on container. do debug on LocalMap's Container_Expand function to find which entity exist
		}
		m_Container_Entity.Empty();
		
		m_Container_AvailiableIndex_Entity.Add(0);
		m_Size_Container_Entity = 1;

		return;
		
	}



	int32 Size_Container_Entity_Original = m_Size_Container_Entity;
	int32 Size_Container_Entity_Target = Size_Container_Entity_Original * 2;

	for (int32 index_Add = 0; index_Add < Size_Container_Entity_Original; ++index_Add) {
		int32 Index_New = Size_Container_Entity_Original + index_Add;
		m_Container_AvailiableIndex_Entity.Add(Index_New);
	}

	m_Size_Container_Entity = Size_Container_Entity_Target;

	

	int32 Num_Container_Entity_After = m_Container_Entity.Num();
	if (Num_Container_Entity_After != Size_Container_Entity_Target) {
		//Exception Entity containers size doesn't match with expected number.
	}

	return;

}

void ALocalMap::Container_Contract_AvailiableIndex_Entity(void)
{
	if (m_Size_Container_Entity <= 0) {
		//Exception Stored entity container size is below 0
		return;
	}

	int32 Size_Container_Entity_Target = m_Size_Container_Entity / 2;
	int32 ID_Entity_Last = 0;

	// HHM Note : Auto Type
	for (auto& EntityPair : m_Container_Entity) {
		int32 ID_Entity = EntityPair.Key;
		if (ID_Entity > ID_Entity_Last) {
			ID_Entity_Last = ID_Entity;
		}
	}

	if (ID_Entity_Last >= Size_Container_Entity_Target) {
		//Exception 
		return;
	}

	/*bool ValidIndex = m_Container_AvailiableIndex_Entity.IsValidIndex(Size_Container_Entity_Target);
	while (ValidIndex == true) {
		m_Container_AvailiableIndex_Entity.RemoveAtSwap()
	}*/

	m_Container_AvailiableIndex_Entity.RemoveAtSwap(Size_Container_Entity_Target, Size_Container_Entity_Target);
}

void ALocalMap::Container_Resize_AvailiableIndex_Entity(void)
{
}

int32 ALocalMap::Entity_Register(AHHM_Entity* _pEntity)
{
	if (_pEntity == nullptr) {
		//Exception Input entity is nullptr
		return -1;
	}

	int32 Size_AvailiableIndex_Entity = m_Container_AvailiableIndex_Entity.Num();
	if (Size_AvailiableIndex_Entity <= 0) {
		int32 Size_Container_Entity = m_Container_Entity.Num();
		if (Size_Container_Entity < Size_AvailiableIndex_Entity) {
			//Exception Size of entity container is smaller than availiable ID queue.there might a hole in entity container.
			//may need some logging option that can debug entity container
			//registering will procceed normaly though.
		}
		
		ALocalMap::Container_Expand_AvailiableIndex_Entity();
	}



	int32 Size_AvailiableIndex_Entity_AfterExpand = m_Container_AvailiableIndex_Entity.Num();
	if (Size_AvailiableIndex_Entity_AfterExpand <= 0) {
		//Exception there is no availiable ID on queue even after expand work procceed. expanding container process might not work correctly.
		return -1;
	}

	int32 Index_FirstAvailiable = m_Container_AvailiableIndex_Entity[0];

	m_Container_AvailiableIndex_Entity.RemoveAt(0);
	bool IsTargetIDOccupied = m_Container_Entity.Contains(Index_FirstAvailiable);

	if (IsTargetIDOccupied == true) {
		//Exception Stored Availiable Index is not Valid. quite surely there were problem on entity creation process.
		return -1;
	}

	m_Container_Entity.Add(Index_FirstAvailiable, _pEntity);

	return Index_FirstAvailiable;	//i just noticed that Available is right not Availiable. :d
}

void ALocalMap::Entity_Deregister(AHHM_Entity* _pEntity)
{
	// HHM Note : There is no contracting container work on deregister process. i may should do shirink container at some point.
	// HHM Note : 어떠한 로깅액션을 취해야 문제가 생긴 사용자에게서 받은 로그와 데이터를 토대로 정확히 같은 상황을 재현하여 원활하게 디버깅이 가능할까?
	// 게임을 로드할때 로드한 시간을 랜덤테이블의 시드로 두고 해당 랜덤테이블상에서 취한 랜덤값을 이용하여 랜덤이벤트등을 진행시키고, 문제 발생시 로드한 세이브와 시간을 저장하게끔?
	// 혹은 저장시마다 시간을 시드로 두고 그 값을 저장하게끔 하여 저장된 데이터를 보내게끔? 플레이어가 직접 저장하는것과는 다르게 백그라운드에서 특정시간마다 갱신되는 세이브로 하는것도
	// 나쁘지 않을듯 싶다. 세이브로드가 불가능한 하드코어모드의 세이브처럼. 또한 오류레벨에 따라 무조건 각자의 파일로 저장할것, 한번 저장한 후에는 일정 시간동안 저장하지 않는것 등으로 구분하는것도
	// 괜찮을듯 싶다. 좀 더 생각해볼것.

	int32 ID_Deregistered = _pEntity->Get_ID();

	AHHM_Entity* StoredEntityAddress = m_Container_Entity[ID_Deregistered];
	if (StoredEntityAddress != _pEntity) {
		//Exception Certain ID's Stored entity data is not matched with Entity that request deregistering of that id
		return;
	}

	m_Container_Entity.Remove(ID_Deregistered);

	int32 Size_Container_AvailiableIndex_Entity = m_Container_AvailiableIndex_Entity.Num();
	for (int32 index_Compare = 0; index_Compare < Size_Container_AvailiableIndex_Entity; ++index_Compare) {
		if (index_Compare > ID_Deregistered) {
			m_Container_AvailiableIndex_Entity.Insert(ID_Deregistered, index_Compare);
			break;
		}
	}

	return;
}

#pragma endregion

#pragma region Navigation

void ALocalMap::Refresh_NavLink(void)
{
	const TArray<FHHM_TileData>& MapData = m_MapData.Container_TileData;
	const FHHM_MapInfo& MapInfo = m_MapInfo;
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

void ALocalMap::Update_NavLink(void)
{
	//HHM Note : Not implemented
}

bool ALocalMap::Check_IsLinkableTile(const TArray<FHHM_TileData>& mapData, const FHHM_MapInfo& mapInfo, int32 checkTileIndex)
{
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

bool ALocalMap::Check_IsTargetableTile(const TArray<FHHM_TileData>& mapData, const FHHM_MapInfo& mapInfo, int32 checkTileIndex)
{
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

bool ALocalMap::Check_IsReachableTile(const TArray<FHHM_TileData>& mapData, const FHHM_MapInfo& mapInfo, int32 originTileIndex, int32 destTileIndex)
{
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

bool ALocalMap::Find_LinkableTile(TArray<int32>* pLinkableTiles, int32 originIndex, const TArray<FHHM_TileData>& mapData, const FHHM_MapInfo& mapInfo)
{
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

bool ALocalMap::Link_Tile(int32* pNavLinkIndex, const int32& originTileIndex, const int32& destTileIndex, const FHHM_MapInfo& mapInfo)
{
	if (pNavLinkIndex == nullptr) {
		//Exception
		return false;
	}

	UWorld* pWorld = GetWorld();
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
	FVector	Location_Origin = FVector::ZeroVector;
	bool IsSucceed_Calculate_Location_Origin = AHHM_Manager_Math_Grid::Convert_Index_To_Translation(Location_Origin, originTileIndex, mapInfo);
	FVector	Location_Destination = FVector::ZeroVector;
	bool IsSucceed_Calculate_Location_Destination = AHHM_Manager_Math_Grid::Convert_Index_To_Translation(Location_Destination, destTileIndex, mapInfo);
	if (IsSucceed_Calculate_Location_Origin == false || IsSucceed_Calculate_Location_Destination == false) {
		//Exception
		return false;
	}

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

void ALocalMap::Refresh_Region(void)
{
	//Get Map Data
	const TArray<FHHM_TileData>&	LocalMapData = m_MapData.Container_TileData;
	const int32						MapSize = LocalMapData.Num();

	//****	Clear Old Region and Initialize it	****//
	m_Container_Tile_Region.Empty();
	m_Container_Tile_Region.Init(-2, MapSize);

	//****	Find all bloced index	****//
	for (int32 CurIndex = 0; CurIndex < MapSize; ++CurIndex) {
		if (LocalMapData[CurIndex].IsPassable == false) {
			m_Container_Tile_Region[CurIndex] = -1;
		}
	}

	//****	find a start point of region and fill it	****//
	int32				NextRegion = 0;
	const FHHM_MapInfo& MapInfo = m_MapInfo;
	for (int32 CurIndex = 0; CurIndex < MapSize; ++CurIndex) {
		if (m_Container_Tile_Region[CurIndex] != -2) {
			continue;
		}

		m_Container_Tile_Region[CurIndex] = NextRegion;
		++NextRegion;

		Fill_EmptyRegion(MapInfo, CurIndex);
	}
}

void ALocalMap::Fill_EmptyRegion(const FHHM_MapInfo& MapInfo, int32 StartIndex)
{
	const int32 CurRegion = m_Container_Tile_Region[StartIndex];
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
			if (m_Container_Tile_Region[Arr_Near[i]] != -2) continue;

			Arr_Fill_IndexQueue.Add(Arr_Near[i]);
		}

		//****	Fill Current Index and remove from queue	****//
		m_Container_Tile_Region[Arr_Fill_IndexQueue[0]] = CurRegion;
		Arr_Fill_IndexQueue.RemoveAt(0);
	}
}

#pragma endregion