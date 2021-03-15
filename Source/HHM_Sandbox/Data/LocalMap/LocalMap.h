// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Header/Struct.h"
#include "Header/Struct_Tile.h"
#include "Data/LocalMap/HHM_Container_SubTileRenderInstance.h"
#include "Header/Struct_Renderer.h"
#include "Header/Struct_LocalMap.h"
#include "Header/Struct_Navigation.h"
#include "Data/Item/ItemData/HHM_ItemData.h"
#include "Header/Enum.h"

#include "Data/Item/ItemData/HHM_ItemData.h"

#include "LocalMap.generated.h"

UCLASS()
class HHM_SANDBOX_API ALocalMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALocalMap();

#pragma region Unreal Method

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginDestroy() override;

#pragma endregion

#pragma region Variables

private:
	UPROPERTY()
		class AHHM_Manager_Tile*		m_pManager_Tile = nullptr;
	UPROPERTY()
		class AHHM_Manager_LocalMap*	m_pManager_LocalMap = nullptr;
	UPROPERTY()
		class AHHM_Manager_Item*		m_pManager_Item = nullptr;



private:
	UPROPERTY(VisibleAnywhere)
		int32							m_ID_LocalMap = -1;
	UPROPERTY()
		int32							m_Index_Horizontal = 0;
	UPROPERTY()
		int32							m_Index_Vertical = 0;
	UPROPERTY()
	//World Location
		FVector2D						m_Location_Offset;
	UPROPERTY()
		FVector2D						m_Vec_Offset;

private:
	UPROPERTY()
		FHHM_MapInfo					m_MapInfo;
	UPROPERTY(EditAnywhere)
		class UBoxComponent*			m_pComponent_TouchPanel = nullptr;
	/*UPROPERTY()
		TArray<FHHM_TileData>			m_Arr_MapData;
	UPROPERTY()
		TArray<FHHM_TileMovementInfo>	m_Arr_MovementData;*/
	UPROPERTY()
		FHHM_LocalMap_MapData			m_MapData;		//MapData Initialized on localmap manager while it creates local map
	//TileMovementData Array

//private:
//	UPROPERTY(VisibleAnywhere)
//		class UHHM_Component_LocalMapRender*	m_pComp_LocalMapRender = nullptr;




	

#pragma endregion

#pragma region Method

#pragma region Initialize related

private:
	bool	Initialize_LocalMap(void);

public:
	void	Validfy_LocalMap(int32 _id, int32 _index_Horizontal, int32 _index_Vertical, const FHHM_MapInfo& _mapInfo);
	bool	Initialize_TouchPanel(void);

private:
	void	Clear_Map(void);
	void	Request_TileManager(void);
	void	Request_ItemManager(void);

	//Movement Data
public:
	void	Refresh_MovementData(void);



	bool	Clear_MeshComponent(void);
	bool	Clear_RenderData(void);

#pragma endregion

#pragma region Get/Set

public:
	UFUNCTION(BlueprintCallable, Category = LocalMap_Data)
		int32							Get_ID_LocalMap() { return m_ID_LocalMap; }
	UFUNCTION(BlueprintCallable, Category = LocalMap_Data)
		FVector2D						Get_IndexLocation_LocalMap() { return FVector2D(m_Index_Horizontal, m_Index_Vertical); }

	FHHM_LocalMap_MapData*			Get_MapData_Address(void) { if (m_ID_LocalMap >= 0) {/*Exception LocalMap is already validfied. can't access to map data's address after validfied*/ } return &m_MapData; }
	UFUNCTION(BlueprintCallable, Category = Variables)
	const FHHM_MapInfo&				Get_MapInfo_ConstRef() const { return m_MapInfo; }
	//Offset representing world location. not local offset
	const FVector2D&				Get_LocalMap_Offset(void) const { if (m_ID_LocalMap == -1) {/*Exception LocalMap Not validfied*/ } return m_Location_Offset; }

	const FHHM_LocalMap_MapData&	Get_MapData(void) const { return m_MapData; }
	FHHM_LocalMap_MapData&			Get_MapData_Ref(void) { return m_MapData; }
	UFUNCTION(BlueprintCallable)
		const FHHM_TileData&		Get_TileInfo(int32 index_Horizontal, int32 index_Vertical) const;
	UFUNCTION(BlueprintCallable)
		const FHHM_TileData&		Get_TileInfo_Const(int32 _index) const;

public:
	//void	Set_MapData(const TArray<FHHM_TileData>& mapData) { m_Arr_MapData = mapData; }

#pragma endregion

#pragma region Interact with map

public:
	//Force to set tile at given position (given tiledata's index variable should be set already).
		void	Set_Tile_At_Pos(int32 _index_Horizontal, int32 _index_Vertical, const FHHM_TileData& _tileData);
	//Force to set tile at given position
	UFUNCTION(BlueprintCallable, Category = TemporaryFunction)
		void	Set_Tile_At_Pos(int32 _index_Horizontal, int32 _index_Vertical, int32 _tile_ID, int32 _tile_SubID);
	//void	Set_Tile_At_Pos_Temp(int32 _index_Horizontal, int32 _index_Vertical, int32 _tileID);

	bool	Place_Tile(int32 _index_Horizontal, int32 _index_Vertical, class AEntity* _pPlacer, FHHM_TileData _tileData);
	bool	Place_Tile(int32 _index_Horizontal, int32 _index_Vertical, class AEntity* _pPlacer, int32 _tile_ID, int32 _tile_SubID);
	
	//Add damage to target tile. Return true if tile destroyed by damage
	bool	Damage_Tile(int32 damage, EHHM_DamageType damage_Type, class APawn* attack_Pawn, int32 index_Horizontal, int32 index_Vertical);

#pragma endregion



#pragma region Location Check

public:
	//check either index location is passable and onGround
	bool	IsLocationStandable(int32 _index, int32 _entitySize_Height) const;
	//check only index location is passable (Unlike 'IsLocationStandable' function)
	bool	IsLocationPassable(int32 _index, int32 _entitySize_Height) const;
	bool	IsOnGround(int32 _index_Horizontal, int32 _index_Vertical) const;
	bool	IsOnGround(int32 _index) const;
	bool	IsAtCeiling(int32 _index_Horizontal, int32 _index_Vertical, int8 _entity_Height = 1) const;
	bool	IsAtCeiling(int32 _index, int8 _entity_Height = 1) const;
	bool	IsOnLadder(int32 _index_Horizontal, int32 _index_Vertical) const;
	bool	IsOnLadder(int32 _index) const;
	bool	IsOnScaffold(int32 _index) const;

	bool	IsTarget_Ladder(int32 _index) const;

	bool	Check_Location_TilePlaceable(int32 _index_Horizontal, int32 _index_Vertical);

	bool	IsAreaPassable(int32 _index, int8 _entity_Width = 1, int8 _entity_Height = 1) const;
	bool	IsAreaStandable(int32 _index, int8 _entity_Width = 1, int8 _entity_Height = 1) const;

#pragma endregion

private:
	bool	Check_IsValidPos(int32 Index_Horizontal, int32 Index_Vertical) const;
	bool	Check_IsLocation_InMap(const FVector& _location) const;
	bool	Check_IsValidIndex(int32 _index) const ;
	void	Translate_TileInfo_To_MovementData(const FHHM_TileData& tileInfo, FHHM_TileMovementInfo& tileMovementInfo);

#pragma endregion



#pragma region TileEntity

	//TArray<TSharedPtr<UHHM_ItemData>> m_Container_TileEntity;

public:
	bool	Get_TileEntity_At(TSharedPtr<AHHM_TileEntity>& _pTileEntity_Return, int32 _index_Horizontal, int32 _index_Vertical);

public:
	bool	Set_TileEntity_At(int32 _index_Horizontal, int32 _index_Vertical, TSharedPtr<AHHM_TileEntity> _pTileEntity);
	
#pragma endregion

#pragma region Rendering

	//Render related
private:
	UPROPERTY(VisibleAnywhere, Category = Rendering)
		int32										m_MaxMapIndex = 0;

	UPROPERTY(VisibleAnywhere, Category = Rendering)
		class UStaticMesh* m_pStaticMesh = nullptr;

	//TileID - TileSubID - TileRenderInstance Index 순으로 타고들어가는 방식.
	//맵(맵(배열(인스턴스 메쉬))) 의 구조로 되어있음.
	UPROPERTY(VisibleAnywhere, Category = Rendering)
		TMap<int32, FHHM_Container_SubTileRenderInstance>		m_Container_RenderInstance;	
	UPROPERTY(VisibleAnywhere, Category = Rendering)
		TArray<FHHM_RenderData>						m_Container_RenderData;

	UPROPERTY(EditAnywhere, Category = Rendering)
		UInstancedStaticMeshComponent* m_pComp_InstancedStaticMesh_Debug = nullptr;



	//Rendering
private:
	bool	Initialize_MeshComponent(void);
	bool	Initialize_Container_InstancedMesh(void);



public:
	UFUNCTION(BlueprintCallable, Category = Rendering)
		bool Render_Reset();

	UFUNCTION(BlueprintCallable, Category = Rendering)
		bool Set_TileRenderData(int32 _index, int32 _tileID, int32 _tileSubID, int32 _tileRenderInstanceIndex, FTransform& _transform);
	UFUNCTION(BlueprintCallable, Category = Rendering)
		//Call this function when certain tile needs to change it's render state. ex) placed,destroyed,etc.
		//해당 인덱스의 타일에게 새로운 렌더상태에 대한 값을 요청하고, 해당 인덱스의 기존 렌더데이터를 삭제후 새로 얻은 렌더값을 렌더합니다.
		//인덱스의 타일에 어떠한 변동사항이 생길경우 호출되어야 합니다.
		bool Update_TileRenderData(const FHHM_TileData& _tileData);

private:
	bool	RenderInstance_Add(int32 _tileID, int32 _tileSubID, int32 _tileRenderInstanceIndex, int32 _index_Tile, FTransform& _tileTransform);
	bool	RenderInstance_Remove(int32 _index_Tile);

	bool	Check_ValidRenderID(int32 _tileID, int32 _tileSubID, int32 _tileRenderInstanceIndex);
	bool	Check_ValidIndex(int32 _index);

#pragma endregion

#pragma region Navigation

private:
	UPROPERTY(VisibleAnywhere)
		TArray<int32>						m_Container_Tile_Region;	//-2 = Not Initialized, -1 = Blocked , 0~ = Region
private:
	UPROPERTY()	//HHM Note :VisibleAnywhere
		TArray<int32>						m_Arr_PendingQueue_LinkUpdate;	//waiting queue to be updated
	UPROPERTY()
		TArray<class AHHM_NavLinkProxy*>	m_Arr_NavLink;
	UPROPERTY()
		TArray<FHHM_NavLinkInfo>			m_Arr_NavLinkInfo;



public:	//Debug Function
	UFUNCTION(BlueprintCallable)
		const TArray<int32>&	Get_Region_Const() const { return m_Container_Tile_Region; }
	UFUNCTION(BlueprintCallable)
		void			Debug_Refresh_Region() { Refresh_Region(); }
	UFUNCTION(BlueprintCallable)
		void			Debug_Refresh_NavLink() { Refresh_NavLink(); }

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

#pragma endregion

#pragma region Entity

	private:
		UPROPERTY()
			int32								m_Size_Container_Entity = 0;
		UPROPERTY()
			TMap<int32, class AHHM_Entity*>		m_Container_Entity;
		//you can just make entity container as list and fill it as nullptr when there is no entity registered on that index(ID).
		//i just use map container because i don't wanna resize id queue so often and resizing entity container itself so i can just resize one container.
		//it might a stupid decision cause due to my current state of my life, i'm so stressed and it ruining my body and my mind so i became much dumber than i was
		//so if you think list would be much simpler or can increase performance much, you can change it.
		UPROPERTY()
			TArray<int32>						m_Container_AvailiableIndex_Entity;	

		

	private:
		void	Container_Expand_AvailiableIndex_Entity(void);
		void	Container_Contract_AvailiableIndex_Entity(void);
		//not sure it is needed. not constructed
		void	Container_Resize_AvailiableIndex_Entity(void); // 2n*2 size / get last index

	public:
		int32	Entity_Register(class AHHM_Entity* _pEntity);
		void	Entity_Deregister(class AHHM_Entity* _pEntity);

	private:
		void	Entity_Clear(void); //Destroy all actor belong this map and empty the container

		//Spawn Entity?
		//Despawn Entity?

#pragma endregion

#pragma region ItemActor

public:
	UFUNCTION(BlueprintCallable, Category = ItemActor)
		bool Spawn_Item_At_Pos_ByID(int32 _index_Horizontal, int32 _index_Vertical, FVector _force_Initial, int32 _item_ID);
	UFUNCTION(BlueprintCallable, Category = ItemActor)
		bool Spawn_Item_At_Pos(int32 _index_Horizontal, int32 _index_Vertical, FVector _force_Initial, UHHM_ItemData* _itemData);
	UFUNCTION(BlueprintCallable, Category = ItemActor)
		bool Spawn_Item_At_Location_ByID(const FVector& _location, const FVector& _force_Initial, int32 _item_ID);
	UFUNCTION(BlueprintCallable, Category = ItemActor)
		bool Spawn_Item_At_Location(const FVector& _location, const FVector& _force_Initial, UHHM_ItemData* _itemData);

#pragma endregion
};
