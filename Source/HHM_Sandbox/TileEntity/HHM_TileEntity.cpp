// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_TileEntity.h"

#include "Tile/Base/HHM_Tile.h"
#include "Data/LocalMap/LocalMap.h"
#include "Components/StaticMeshComponent.h"

#include "Header/Macro.h"



// Sets default values
AHHM_TileEntity::AHHM_TileEntity()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if(RootComponent == nullptr){
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Component_Scene"));
	}
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



bool AHHM_TileEntity::Component_StaticMesh_Add(const FString& _name, UMaterialInterface* _pMaterial, const FVector2D& _size, const FVector2D& _offset, const float& _offset_Depth)
{
	if (_pMaterial == nullptr) {
		//Exception
		return false;
	}

	bool IsNameTaken = m_Container_Component_StaticMesh.Contains(_name);
	if (IsNameTaken == true) {
		//Exception Notice Name Taken
		return false;
	}

	if (_size.X <= 0 || _size.Y <= 0) {
		//Exception
		return false;
	}



	//static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset_StaticMesh(HHM_PATH_MESH_TILE);
	UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(nullptr, HHM_PATH_MESH_TILE);
	if (IsValid(StaticMesh) == true) {
		FString ComponentName = FString::Printf(TEXT("Component_StaticMesh_%s"), *_name);
		//UStaticMeshComponent* pComponent_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(*ComponentName);
		UStaticMeshComponent* pComponent_StaticMesh = NewObject<UStaticMeshComponent>(this, *ComponentName);
		if (pComponent_StaticMesh == nullptr) {
			//Exception
			return false;
		}

		pComponent_StaticMesh->SetupAttachment(RootComponent);
		pComponent_StaticMesh->SetStaticMesh(StaticMesh);
		pComponent_StaticMesh->SetMaterial(0, _pMaterial);

		FVector2D MeshSize = FVector2D((HHM_TILE_SIZE / HHM_TILE_MESH_SIZE) * _size.X, (HHM_TILE_SIZE / HHM_TILE_MESH_SIZE) * _size.Y);
		pComponent_StaticMesh->SetWorldScale3D(FVector(MeshSize.X, 0.001f, MeshSize.Y));
		pComponent_StaticMesh->SetRelativeLocation(FVector(_offset.X, _offset_Depth, _offset.Y));

		pComponent_StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);



		pComponent_StaticMesh->RegisterComponent();

		m_Container_Component_StaticMesh.Add(_name, pComponent_StaticMesh);

		return true;
	}

	return false;
}

bool AHHM_TileEntity::Component_StaticMesh_Remove(const FString& _name)
{
	bool IsNameExist = m_Container_Component_StaticMesh.Contains(_name);
	if (IsNameExist == false) {
		//Exception
		return false;
	}

	//m_Container_Component_StaticMesh[_name]->DestroyComponent();
	m_Container_Component_StaticMesh.Remove(_name);

	return true;
}

bool AHHM_TileEntity::Get_StaticMeshComponent(const FString& _name, UStaticMeshComponent*& _pStaticMesh_Return)
{
	bool IsNameExist = m_Container_Component_StaticMesh.Contains(_name);
	if (IsNameExist == false) {
		//Exception
		return false;
	}

	_pStaticMesh_Return = m_Container_Component_StaticMesh[_name];

	return true;
}

