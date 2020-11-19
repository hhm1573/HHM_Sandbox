// Fill out your copyright notice in the Description page of Project Settings.

#include "HHM_Manager_Render.h"

#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Base/GameInstance/HHM_GameInstance.h"
#include "Components/InstancedStaticMeshComponent.h"

#include "Manager/Tile/HHM_Manager_Tile.h"
#include "Tile/Base/HHM_Tile.h"

#include "Header/Struct_Renderer.h"

#include "Manager/Math/Grid/HHM_Manager_Math_Grid.h"

#include "Engine/StaticMesh.h"

#include "GameFramework/GameModeBase.h"
#include "Base/GameMode/HHM_GameMode_LocalMap.h"

const FVector Const_TileOffsetH = FVector(50.0f, 0.0f, 0.0f);



void AHHM_Manager_Render::BeginPlay() {
	Super::BeginPlay();
	//SetActorLocation(FVector(50.0f, 0.0f, 50.0f));
	Update_MapInfo();
	Initialize_MeshComponent();
	Initialize_Storage_InstancedMesh();
}



void AHHM_Manager_Render::Update_MapInfo(void) {
	UHHM_GameInstance*	pGameInstance = Cast<UHHM_GameInstance>(AHHM_Manager::m_pGameInstance);
	if (pGameInstance == nullptr) {
		return;
	}

	m_MapInfo = pGameInstance->Get_MapInfo();
	m_MaxMapIndex = (m_MapInfo.MapSize_Horizontal * m_MapInfo.MapSize_Vertical) - 1;
}

void AHHM_Manager_Render::Set_RenderMapData(const TArray<FHHM_TileData>& Arr_TileInfo) {	//타일의 서브아이디를 제대로 받아오는 부분이 없기에 추후에 타일에게서 서브아이디를 받아오는 함수를 호출하는 작업의 추가가 필요함.
	int32	ArrSize_Input = Arr_TileInfo.Num();
	int32	ArrSize_Mesh = m_Arr_pInstancedMesh.Num();

	//Clear RenderData Array and Copy data from Input TileInfo Data
	Clear_RenderData();
	for (int32 CurArrIndex = 0; CurArrIndex < ArrSize_Input; ++CurArrIndex) {
		//FHHM_TileRenderInfo		TileRenderInfo = FHHM_TileRenderInfo(Arr_TileInfo[CurArrIndex].ID, Arr_TileInfo[CurArrIndex].);
		FHHM_RenderData		TileRenderInfo = FHHM_RenderData();
		TileRenderInfo.Tile_ID = Arr_TileInfo[CurArrIndex].ID;
		TileRenderInfo.Tile_SubID = 0;
		m_Arr_RenderInfo.Add(TileRenderInfo);
	}

	//Check is RenderArray's size is same as InputArray's size. just in case
	int32	ArrSize_Render = m_Arr_RenderInfo.Num();
	if (ArrSize_Input != ArrSize_Render) {
		//Exception
		return;
	}

	//Clear Instances of Mesh and fill with new Render data
	Clear_MeshComponent();
	for (int32 CurArrIndex = 0; CurArrIndex < ArrSize_Render; ++CurArrIndex) {
		if (Check_ValidTileID(m_Arr_RenderInfo[CurArrIndex].Tile_ID, 0) == false) {
			//Exception
			return;
		}

		/*FVector		TileTranslation = AHHM_Manager_Math_Grid::Convert_IndexToTranslation(m_MapInfo, CurArrIndex) + Const_TileOffset;
		FTransform	TileTransform = FTransform();
		TileTransform.SetTranslation(TileTranslation);

		int32		InstanceIndex = m_Arr_pInstancedMesh[m_Arr_RenderInfo[CurArrIndex].Tile_ID]->AddInstance(TileTransform);
		m_Arr_RenderInfo[CurArrIndex].Render_Index = InstanceIndex;*/
		FTransform Transform = FTransform();
		RenderInstance_Add(m_Arr_RenderInfo[CurArrIndex].Tile_ID, 0, CurArrIndex, Transform);
		
		if (m_Arr_RenderInfo[CurArrIndex].Render_Index < 0) {
			//Exception
			return;
		}
	}

	UpdateComponentTransforms();

	/*for (int32 CurArrIndex = 0; CurArrIndex < ArrSize_Input; ++CurArrIndex) {
		if (Check_ValidTileID(Arr_TileInfo[CurArrIndex].Tile_ID) == false) {	//Check TileID is valid for rendering
			//Todo : Some Log work here
			continue;
		}

		int32	TileIndex_Horizontal = CurArrIndex % m_MapInfo.MapSize_Horizontal;
		int32	TileIndex_Vertical = CurArrIndex / m_MapInfo.MapSize_Horizontal;
		float	TilePos_Horizontal = float(TileIndex_Horizontal) * m_MapInfo.TileSize;
		float	TilePos_Vertical = float(TileIndex_Vertical) * m_MapInfo.TileSize;

		FVector	VecTilePos = FVector(TilePos_Horizontal, 0.0f, TilePos_Vertical);
		FTransform	Transform_Tile = FTransform();
		Transform_Tile.SetTranslation(VecTilePos);
		
		m_Arr_pInstancedMesh[Arr_TileInfo[CurArrIndex].Tile_ID]->AddInstance(Transform_Tile);
	}*/
}

void AHHM_Manager_Render::Set_TileRenderData(int32 index, int32 tileID, int32 tileSubID, FTransform transform) {
	const bool isValidIndex = Check_ValidIndex(index);
	if (isValidIndex == false) {
		//Exception
		return;
	}

	const bool isValidTileID = Check_ValidTileID(tileID, tileSubID);
	if (isValidTileID == false) {
		//Exception
		return;
	}



	const bool Succeed_RemoveInstance = RenderInstance_Remove(index);
	if (Succeed_RemoveInstance == false) {
		//Exception
		return;
	}
	
	const bool Succeed_AddInstance = RenderInstance_Add(tileID, tileSubID, index, transform);
	if (Succeed_AddInstance == false) {
		//Exception
		return;
	}
}

void AHHM_Manager_Render::Update_RenderInfo_Tile(class ALocalMap* pLocalMap, FHHM_TileData& tileInfo) {
	//null check
	if (pLocalMap == nullptr) {
		//Exception
		return;
	}

	if (tileInfo.Tile == nullptr) {
		//Exception
		return;
	}

	//Get essential information to update render
	FTransform	Transform_Local = FTransform();
	int32		index_instance = -1;

	//Call Tile's update_Render function
	const bool bIsUpdateSuccess = tileInfo.Tile->Update_Render(index_instance, Transform_Local, pLocalMap, tileInfo);

	//if render info updating job is needed, do update
	if (bIsUpdateSuccess == true) {
		const int32 index_Update = AHHM_Manager_Math_Grid::Index_Combine(tileInfo.Index_Horizontal, tileInfo.Index_Vertical, m_MapInfo);
		
		const bool isRemoveSucceed = RenderInstance_Remove(index_Update);
		if (isRemoveSucceed == false) {
			//Exception
			return;
		}

		const bool isAddSucceed = RenderInstance_Add(tileInfo.ID, index_instance, index_Update, Transform_Local);
		if (isAddSucceed == false) {
			//Exception
			return;
		}
	}
}



void AHHM_Manager_Render::Initialize_MeshComponent(void) {
	UInstancedStaticMeshComponent* pInstancedMeshComponent = nullptr;
	pInstancedMeshComponent = NewObject<UInstancedStaticMeshComponent>(this, TEXT("InstancedMesh_0"));
	m_Arr_pInstancedMesh.Add(pInstancedMeshComponent);
	m_Arr_pInstancedMesh[0]->RegisterComponent();

	m_Arr_pInstancedMesh.Add(NewObject<UInstancedStaticMeshComponent>(this, TEXT("InstancedMesh_1")));
	m_Arr_pInstancedMesh[1]->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Cube")));
	m_Arr_pInstancedMesh[1]->RegisterComponent();

	m_Arr_pInstancedMesh.Add(NewObject<UInstancedStaticMeshComponent>(this, TEXT("InstancedMesh_2")));
	m_Arr_pInstancedMesh[2]->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Sphere")));
	m_Arr_pInstancedMesh[2]->RegisterComponent();

	m_pStaticMesh_Plane = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
}

void AHHM_Manager_Render::Initialize_Storage_InstancedMesh(void) {
	//타일매니저에서 타일 다 받아서 인스턴스마다 맵에 넣으면됨

	//Check require objects
	if (m_pStaticMesh_Plane == nullptr) {
		//Exception
		return;
	}

	//Get Tile Manager
	/*UGameInstance*	pGameInstance_Raw = GetGameInstance();
	if (pGameInstance_Raw == nullptr) {
		//Exception
		return;
	}

	UHHM_GameInstance*	pGameInstance = Cast<UHHM_GameInstance>(pGameInstance_Raw);
	if (pGameInstance == nullptr) {
		//Exception
		return;
	}

	AHHM_Manager_Tile*	pManager_Tile = pGameInstance->Get_Manager_Tile();
	if (pManager_Tile == nullptr) {
		//Exception
		return;
	}*/
	AHHM_GameMode_LocalMap* pGameMode = Cast<AHHM_GameMode_LocalMap>(GetWorld()->GetAuthGameMode());
	AHHM_Manager_Tile* pManager_Tile = pGameMode->Get_Manager_Tile();	//Temporary Debug Part
	

	//Get Tile Container
	TMap<int32, AHHM_Tile*>& Ref_TileContainer = pManager_Tile->Get_TileArr_Ref();

	//iterate container and if tile's render type is 'instanced', create render info based on tile's info
	const int32 Num_TileContainer = Ref_TileContainer.Num();

	for (int32 index_Tile = 0; index_Tile < Num_TileContainer; ++index_Tile) {
		if (Ref_TileContainer[index_Tile] == nullptr) {
			//Exception
			continue;
		}
		
		//Get Tile's RenderInfo data
		const FHHM_RenderInfo&	TileRenderInfo = Ref_TileContainer[index_Tile]->Get_RenderInfo();
		if (TileRenderInfo.eRenderType != EHHM_RenderType::RType_Instanced) {
			continue;
		}

		//Check Tile is already registered
		bool IsTileAlreadyRegistered = m_Map_Instanced.Contains(Ref_TileContainer[index_Tile]->Get_TileID());
		if (IsTileAlreadyRegistered == true) {
			//Exception
			continue;
		}

		//make some place on map for tile
		m_Map_Instanced.Add(index_Tile, FHHM_InstancedMeshArray());

		//Filling render manager's instance map based on tile's material
		int32 Num_TileMaterial = TileRenderInfo.Num_Material;
		for (int32 index_Material = 0; index_Material < Num_TileMaterial; ++index_Material) {

			//Check tile's material is already registered
			int32 Num_Registered_Material = m_Map_Instanced[index_Tile].Arr_pInstancedStaticMesh.Num();
			if (index_Material < Num_Registered_Material) {
				//Exception;
				continue;
			}

			//Check is material is availiable
			if (TileRenderInfo.Arr_Material[index_Material] == nullptr) {
				//Exception
				return;
			}

			//Create Mesh Component than set mesh and material
			FString Name_InstancedMesh = FString::Printf(TEXT("InstancedMesh_%d_%d"),index_Tile, Num_TileMaterial);
			UInstancedStaticMeshComponent*	pInstancedMeshComponent = nullptr;
			pInstancedMeshComponent = NewObject<UInstancedStaticMeshComponent>(this, *Name_InstancedMesh);
			pInstancedMeshComponent->SetStaticMesh(m_pStaticMesh_Plane);
			pInstancedMeshComponent->SetMaterial(0, TileRenderInfo.Arr_Material[index_Material]);

				//Collision Setting
			const bool IsPassable = Ref_TileContainer[index_Tile]->Get_IsPassable();
			if (IsPassable) {
				pInstancedMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
			}
			else {
				pInstancedMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
				const ECollisionEnabled::Type e = pInstancedMeshComponent->GetCollisionEnabled();
				int i = 0;
			}

				//Rendering Setting
			const bool IsNeedToRender = TileRenderInfo.IsNeedToRender;
			if (IsNeedToRender == false) {
				pInstancedMeshComponent->bHiddenInGame = true;
			}

			//Register instanced static mesh component on renderer's map
			m_Map_Instanced[index_Tile].Arr_pInstancedStaticMesh.Add(pInstancedMeshComponent);
			m_Map_Instanced[index_Tile].Arr_pInstancedStaticMesh[index_Material]->RegisterComponent();
		}
	}
}

void AHHM_Manager_Render::Clear_MeshComponent(void) {
	int32 ArrSize = m_Arr_pInstancedMesh.Num();
	for (int32 i = 0; i < ArrSize; ++i) {
		m_Arr_pInstancedMesh[i]->ClearInstances();
	}

	for (auto& pair : m_Map_Instanced) {
		for (UInstancedStaticMeshComponent* pInstancedMesh : pair.Value.Arr_pInstancedStaticMesh) {
			if (pInstancedMesh != nullptr) {
				pInstancedMesh->ClearInstances();
			}
		}
	}
}

void AHHM_Manager_Render::Clear_RenderData(void) {
	m_Arr_RenderInfo.Empty();
}

//void AHHM_Manager_Render::RenderInstance_Add(int32 tileID, int32 tileSubID, int32 index_Tile) {
//	const bool isValidTileIndex = Check_ValidIndex(index_Tile);
//	if (isValidTileIndex == false) {
//		//Exception
//		return;
//	}
//
//}

bool AHHM_Manager_Render::RenderInstance_Add(int32 tileID, int32 tileSubID, int32 index_Tile, FTransform& tileTransform) {
	const bool isValidTileID = Check_ValidTileID(tileID, tileSubID);
	if (isValidTileID == false) {
		//Exception
		return false;
	}

	const bool isValidTileIndex = Check_ValidIndex(index_Tile);
	if (isValidTileIndex == false) {
		//Exception
		return false;
	}

	//Adjusting target tile's translation
	FTransform	Transform_Adjusted = tileTransform;
	FVector		Translation_Raw = FVector::ZeroVector;	//Before add offset coordinate
	bool IsSucceed_ConvertTranslation = AHHM_Manager_Math_Grid::Convert_Index_To_Translation(Translation_Raw, index_Tile, m_MapInfo);
	if (IsSucceed_ConvertTranslation == false) {
		//Exception
		return false;
	}
	FVector		Translation = Translation_Raw + Const_TileOffsetH;
	Transform_Adjusted.SetTranslation(Translation);	

	//Add instance and save index to renderinfo
	const int32 index_Instance = m_Map_Instanced[tileID].Arr_pInstancedStaticMesh[tileSubID]->AddInstance(Transform_Adjusted);
	m_Arr_RenderInfo[index_Tile].Render_Index = index_Instance;
	m_Arr_RenderInfo[index_Tile].Tile_ID = tileID;
	m_Arr_RenderInfo[index_Tile].Tile_SubID = tileSubID;

	return true;
}

bool AHHM_Manager_Render::RenderInstance_Remove(int32 index_Tile) {
	//Check input value is valid
	const bool isValidTileIndex = Check_ValidIndex(index_Tile);
	if (isValidTileIndex == false) {
		//Exception
		return false;
	}

	//Check if target index don't have any render info
	if (m_Arr_RenderInfo[index_Tile].Render_Index < 0) {
		return false;
	}

	const int32 TileID_Target = m_Arr_RenderInfo[index_Tile].Tile_ID;
	const int32 TileSubID_Target = m_Arr_RenderInfo[index_Tile].Tile_SubID;

	//Check renderinfo's tileID is valid
	const bool isValidTileID = Check_ValidTileID(TileID_Target, TileSubID_Target);
	if (isValidTileID == false) {
		//Exception
		return false;
	}



	//Prepare some variables for checking work
	const int32 Num_Instance_TargetMeshComp = m_Map_Instanced[TileID_Target].Arr_pInstancedStaticMesh[TileSubID_Target]->GetNumRenderInstances();
	const int32 index_Instance_Last = Num_Instance_TargetMeshComp - 1;
	const int32 index_Instance_Target = m_Arr_RenderInfo[index_Tile].Render_Index;

	//Check if target(removing instance) is the last instance of target instancedMeshComp. if it isn't the last one, copy the last instance to target's info.
	if (index_Instance_Target != index_Instance_Last) {
		//Get the last instance's transform
		FTransform Transform_LastInstance = FTransform();
		if (m_Map_Instanced[TileID_Target].Arr_pInstancedStaticMesh[TileSubID_Target]->GetInstanceTransform(index_Instance_Last, Transform_LastInstance) == false) {
			//Exception - cant get transform of last instance
		}

		//Find last instance's index
		FVector	Translation_LastInstance = Transform_LastInstance.GetTranslation();
		int32	index_LastInstance = -1;
		bool IsSucceed_ConvertToIndex = AHHM_Manager_Math_Grid::Convert_Translation_To_Index(index_LastInstance, Translation_LastInstance, m_MapInfo);
		if (IsSucceed_ConvertToIndex == false) {
			//Exception
			return false;
		}

		//Paste last instance's transform data to target's instance
		m_Map_Instanced[TileID_Target].Arr_pInstancedStaticMesh[TileSubID_Target]->UpdateInstanceTransform(index_Instance_Target, Transform_LastInstance);
		//Update renderinfo of last instance's index
		m_Arr_RenderInfo[index_LastInstance].Render_Index = index_Instance_Target;
	}

	//Delete last instance of target instanceMeshComp
	m_Map_Instanced[TileID_Target].Arr_pInstancedStaticMesh[TileSubID_Target]->RemoveInstance(index_Instance_Last);

	return true;
}




bool AHHM_Manager_Render::Check_ValidTileID(int32 tileID, int32 tileSubID) {
	if (tileID < 0)	return false;
	if (tileSubID < 0) return false;

	const bool isIDRegistered = m_Map_Instanced.Contains(tileID);
	if (isIDRegistered == false) {
		return false;
	}

	const int32 Num_SubID = m_Map_Instanced[tileID].Arr_pInstancedStaticMesh.Num();
	if (tileSubID >= Num_SubID) {
		return false;
	}

	return true;
}

bool AHHM_Manager_Render::Check_ValidIndex(int32 index) {
	if (index < 0) return false;

	if (index > m_MaxMapIndex) return false;

	return true;
}