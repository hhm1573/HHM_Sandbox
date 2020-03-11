// Fill out your copyright notice in the Description page of Project Settings.

#include "TouchPanel.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Base/GameMode/HHM_GameMode_LocalMap.h"

#include "Engine/GameInstance.h"
#include "Base/GameInstance/HHM_GameInstance.h"
#include "Header/Struct.h"

// Sets default values
ATouchPanel::ATouchPanel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if (m_pComponent_Box == nullptr) {
		m_pComponent_Box = NewObject<UBoxComponent>(this, TEXT("BoxComponent"));
		if (m_pComponent_Box == nullptr) {
			//Exception
		}
		RootComponent = m_pComponent_Box;
	}

	if (m_pComponent_StaticMesh == nullptr) {
		m_pComponent_StaticMesh = NewObject<UStaticMeshComponent>(this, TEXT("StaticMeshComponent"));
		if (m_pComponent_StaticMesh == nullptr) {
			//Exception
		}
		m_pComponent_StaticMesh->SetupAttachment(m_pComponent_Box);
	}
}

// Called when the game starts or when spawned
void ATouchPanel::BeginPlay()
{
	Super::BeginPlay();


	
	//Setup Components
	if (m_pComponent_Box == nullptr || m_pComponent_StaticMesh == nullptr) {
		//Exception
		return;
	}

	//Get MapInfo
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

	const FHHM_MapInfo& MapInfo = pGameMode->Get_MapInfo();

	float	Size_Horizontal = MapInfo.MapSize_Horizontal * MapInfo.TileSize;
	float	Size_Vertical = MapInfo.MapSize_Vertical * MapInfo.TileSize;
	FVector	Size_Panel = FVector(Size_Horizontal, 0.0f, Size_Vertical);

	m_pComponent_Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	m_pComponent_Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	m_pComponent_Box->SetBoxExtent(Size_Panel * 0.5f);

	UStaticMesh* pStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Plane"));
	FVector	Size_MeshPanel = FVector(Size_Horizontal, Size_Vertical, 1.0f) * 0.01f;
	if (pStaticMesh != nullptr) {
		m_pComponent_StaticMesh->SetStaticMesh(pStaticMesh);
	}
	FQuat	Rotation = FQuat::MakeFromEuler(FVector(90.0f, 0.0f, 0.0f));
	m_pComponent_StaticMesh->SetWorldRotation(Rotation);
	m_pComponent_StaticMesh->SetWorldScale3D(Size_MeshPanel);
	m_pComponent_StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	SetActorLocation(Size_Panel * 0.5f);

	m_pComponent_StaticMesh->bHiddenInGame = true;
}

// Called every frame
void ATouchPanel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



//void ATouchPanel::BeginPlay()
//{
//	Super::BeginPlay();
//
//
//
//	//Setup Components
//	if (m_pComponent_Box == nullptr || m_pComponent_StaticMesh == nullptr) {
//		//Exception
//		return;
//	}
//
//	UGameInstance* pGameInstance_Raw = GetGameInstance();
//	if (pGameInstance_Raw == nullptr) {
//		//Exception
//		return;
//	}
//	UHHM_GameInstance* pGameInstance = Cast<UHHM_GameInstance>(pGameInstance_Raw);
//	if (pGameInstance == nullptr) {
//		//Exception
//		return;
//	}
//	FHHM_MapInfo MapInfo = pGameInstance->Get_MapInfo();
//
//	float	Size_Horizontal = MapInfo.MapSize_Horizontal * MapInfo.TileSize;
//	float	Size_Vertical = MapInfo.MapSize_Vertical * MapInfo.TileSize;
//	FVector	Size_Panel = FVector(Size_Horizontal, 0.0f, Size_Vertical);
//
//	m_pComponent_Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
//	m_pComponent_Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
//	m_pComponent_Box->SetBoxExtent(Size_Panel * 0.5f);
//
//	UStaticMesh* pStaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Plane"));
//	FVector	Size_MeshPanel = FVector(Size_Horizontal, Size_Vertical, 1.0f) * 0.01f;
//	if (pStaticMesh != nullptr) {
//		m_pComponent_StaticMesh->SetStaticMesh(pStaticMesh);
//	}
//	FQuat	Rotation = FQuat::MakeFromEuler(FVector(90.0f, 0.0f, 0.0f));
//	m_pComponent_StaticMesh->SetWorldRotation(Rotation);
//	m_pComponent_StaticMesh->SetWorldScale3D(Size_MeshPanel);
//	m_pComponent_StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
//
//	SetActorLocation(Size_Panel * 0.5f);
//
//	m_pComponent_StaticMesh->bHiddenInGame = true;
//}