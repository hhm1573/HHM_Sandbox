// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_ItemActor.h"

#include "Components/StaticMeshComponent.h"
#include "Header/Macro.h"

#include "UObject/ConstructorHelpers.h"



// Sets default values
AHHM_ItemActor::AHHM_ItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	UStaticMeshComponent* pComponent_StaticMesh = nullptr;
	pComponent_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	if (pComponent_StaticMesh == nullptr) {
		//Exception
	}
	else {
		static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh_Asset(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
		if (StaticMesh_Asset.Succeeded() == true) {
			pComponent_StaticMesh->SetStaticMesh(StaticMesh_Asset.Object);
			m_pComponent_StaticMesh = pComponent_StaticMesh;
			RootComponent = m_pComponent_StaticMesh;
			//m_pComponent_StaticMesh->SetupAttachment(RootComponent);
			m_pComponent_StaticMesh->SetSimulatePhysics(true);
		}
		else {
			//Exception No Mesh Asset Found
		}
	}
}

// Called when the game starts or when spawned
void AHHM_ItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHHM_ItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AHHM_ItemActor::Set_ItemData(const TSharedPtr<UHHM_ItemData>& _pItemData, UMaterialInterface* _material, FVector2D _actorSize)
{
	if (_pItemData == nullptr) {
		//Exception
		return false;
	}

	if (_pItemData->Get_Item() == nullptr) {
		//Exception
		return false;
	}

	if (_material == nullptr) {
		//Exception
		return false;
	}

	if (_actorSize.X <= 0 || _actorSize.Y <= 0) {
		//Exception
		return false;
	}



	m_pItemData = _pItemData;
	m_pComponent_StaticMesh->SetMaterial(0, _material);
	float Scale_X = _actorSize.X / HHM_CUBE_MESH_SIZE;
	float Scale_Y = _actorSize.Y / HHM_CUBE_MESH_SIZE;
	m_pComponent_StaticMesh->SetWorldScale3D(FVector(Scale_X, 0.05f, Scale_Y));



	return true;
}

void AHHM_ItemActor::Add_Force(FVector _force)
{
	m_pComponent_StaticMesh->AddForce(_force);
}

