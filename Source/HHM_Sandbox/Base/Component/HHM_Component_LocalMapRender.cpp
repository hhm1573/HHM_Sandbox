// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Component_LocalMapRender.h"

#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Base/GameMode/HHM_GameMode_LocalMap.h"
#include "Data/LocalMap/LocalMap.h"
#include "Manager/Tile/HHM_Manager_Tile.h"

#include "Tile/Base/HHM_Tile.h"

#include "Manager/Math/Grid/HHM_Manager_Math_Grid.h"

#include "Components/InstancedStaticMeshComponent.h"

const FVector Const_TileOffset = FVector(50.0f, 0.0f, 0.0f);



UHHM_Component_LocalMapRender::UHHM_Component_LocalMapRender() {

}



bool UHHM_Component_LocalMapRender::Set_MapData(const TArray<FHHM_TileData>& _arr_TileInfo, const FHHM_MapInfo& _mapInfo)
{
	m_MapInfo = _mapInfo;
	m_MaxMapIndex = ( _mapInfo.MapSize_Horizontal * _mapInfo.MapSize_Vertical ) - 1;

	int32	ArrSize_Input = _arr_TileInfo.Num();

	//Clear RenderData Array and Copy data from Input TileInfo Data
	Clear_RenderData();
	for (int32 CurArrIndex = 0; CurArrIndex < ArrSize_Input; ++CurArrIndex) {
		FHHM_RenderData		TileRenderData = FHHM_RenderData();
		TileRenderData.Tile_ID = _arr_TileInfo[CurArrIndex].ID;
		TileRenderData.Tile_SubID = _arr_TileInfo[CurArrIndex].SubID;
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

	UpdateChildTransforms();

	return true;
}

bool UHHM_Component_LocalMapRender::Set_TileRenderData(int32 _index, int32 _tileID, int32 _tileSubID, FTransform& _transform)
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

bool UHHM_Component_LocalMapRender::Update_TileRenderData(ALocalMap* _pLocalMap, const FHHM_TileData& _tileData)
{
	//null check
	if (_pLocalMap == nullptr) {
		//Exception
		return false;
	}

	if (_tileData.Tile == nullptr) {
		//Exception
		return false;
	}

	//Get essential information to update render
	FTransform	Transform_Local = FTransform();
	int32		index_instance = -1;

	//Call Tile's update_Render function		to get essential info(tranform_local, index_instance)
	const bool bIsUpdateSuccess = _tileData.Tile->Update_Render(index_instance, Transform_Local, _pLocalMap, _tileData);

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



bool UHHM_Component_LocalMapRender::Initialize_Renderer(void) {
	const bool Succeed_Initialize_MeshComponent = Initialize_MeshComponent();
	if (Succeed_Initialize_MeshComponent == false) {
		return false;
	}

	const bool Succeed_Initialize_MeshContainer = Initialize_Container_InstancedMesh();
	if (Succeed_Initialize_MeshContainer == false) {
		return false;
	}

	return true;
}

bool UHHM_Component_LocalMapRender::Initialize_MeshComponent(void)
{
	m_pStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
	if (m_pStaticMesh == nullptr) {
		//Exception
		return false;
	}

	return true;
}

bool UHHM_Component_LocalMapRender::Initialize_Container_InstancedMesh(void)
{
	//Test
	ALocalMap* pLocalMap = nullptr;
	pLocalMap = Cast<ALocalMap>(GetOwner());
	if (pLocalMap == nullptr) {
		//Exception
		return false;
	}



	//-------- Check Required Component --------//
	if (m_pStaticMesh == nullptr) {
		//Exception
		return false;
	}

	//-------- Get Tile Manager to get Tile container ---------//
	UWorld* pWorld = nullptr;
	pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception
		return false;
	}

	AGameModeBase* pGameMode_Raw = nullptr;
	pGameMode_Raw = pWorld->GetAuthGameMode();
	if (pGameMode_Raw == nullptr) {
		//Exception
		return false;
	}

	AHHM_GameMode_LocalMap* pGameMode = nullptr;
	pGameMode = Cast<AHHM_GameMode_LocalMap>(pGameMode_Raw);
	if (pGameMode == nullptr) {
		//Exception
		return false;
	}

	AHHM_Manager_Tile* pManager_Tile = nullptr;
	pManager_Tile = pGameMode->Get_Manager_Tile();
	if (pManager_Tile == nullptr) {
		//Exception
		return false;
	}
	
	TMap<int32, AHHM_Tile*>& Ref_TileContainer = pManager_Tile->Get_TileArr_Ref();



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
			pInstancedStaticMeshComponent->SetupAttachment(this);
			pInstancedStaticMeshComponent->SetStaticMesh(m_pStaticMesh);
			pInstancedStaticMeshComponent->SetMaterial(0, TileRenderInfo.Arr_Material[index_Material]);

			//Collision Setting
			const bool IsPassable = Ref_TileContainer[index_Tile]->Get_IsPassable();
			if (IsPassable) {
				pInstancedStaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
			}
			else {
				pInstancedStaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
			}

			//Rendering Setting
			const bool IsNeedToRender = TileRenderInfo.IsNeedToRender;
			if (IsNeedToRender == false) {
				pInstancedStaticMeshComponent->bHiddenInGame = true;
			}

			//Register instanced static mesh component on renderer's map
			m_Container_Comp_InstancedMesh[index_Tile].Arr_pInstancedStaticMesh.Add(pInstancedStaticMeshComponent);
			//m_Container_Comp_InstancedMesh[index_Tile].Arr_pInstancedStaticMesh[index_Material]->RegisterComponent();
		}
	}

	m_Temp_InstancedMeshComp = m_Container_Comp_InstancedMesh[1].Arr_pInstancedStaticMesh[0];

	return true;
}



bool UHHM_Component_LocalMapRender::Clear_MeshComponent(void)
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

bool UHHM_Component_LocalMapRender::Clear_RenderData(void)
{
	m_Container_RenderData.Empty();
	return true;
}



bool UHHM_Component_LocalMapRender::RenderInstance_Add(int32 _tileID, int32 _tileSubID, int32 _index_Tile, FTransform& _tileTransform)
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
	FVector		Translation_Raw = FVector();
	bool IsSucceed_CalculateTranslation = AHHM_Manager_Math_Grid::Convert_Index_To_Translation(Translation_Raw, _index_Tile, m_MapInfo);
	if (IsSucceed_CalculateTranslation == false) {
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

bool UHHM_Component_LocalMapRender::RenderInstance_Remove(int32 _index_Tile)
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
		bool IsSucceed_Calculate_Index = AHHM_Manager_Math_Grid::Convert_Translation_To_Index(index_LastInstance, Translation_LastInstance, m_MapInfo);
		if (IsSucceed_Calculate_Index == false) {
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



bool UHHM_Component_LocalMapRender::Check_ValidRenderID(int32 _tileID, int32 _tileSubID)
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

bool UHHM_Component_LocalMapRender::Check_ValidIndex(int32 _index)
{
	if (_index < 0) return false;

	if (_index > m_MaxMapIndex) return false;

	return true;
}
