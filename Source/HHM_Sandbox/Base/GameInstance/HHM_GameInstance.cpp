// Fill out your copyright notice in the Description page of Project Settings.

#include "HHM_GameInstance.h"

#include "Engine/World.h"
#include "Manager/HHM_Manager.h"

#include "Manager/Texture/HHM_Manager_Texture.h"

#include "Manager/Render/HHM_Manager_Render.h"
//#include "Manager/LocalMap/HHM_Manager_LocalMap.h"
#include "Manager/Tile/HHM_Manager_Tile.h"
#include "Manager/Navigation/HHM_Manager_Navigation.h"

#include "Header/Macro.h"

//#include "Manager/Debug/HHM_Manager_Debug.h"

#include "Base/TouchPanel/TouchPanel.h"



void UHHM_GameInstance::Initialize_Game_Implementation() {
	Spawn_Manager();
}



void UHHM_GameInstance::Spawn_Manager(void) {
	m_MapInfo.MapSize_Horizontal = 64;
	m_MapInfo.MapSize_Vertical = 64;
	m_MapInfo.TileSize = HHM_TILE_SIZE;

	UWorld*	pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception
		return;
	}

	//m_pManager_Texture = pWorld->SpawnActor<AHHM_Manager_Texture>();

	//m_pManager_Tile = pWorld->SpawnActor<AHHM_Manager_Tile>();

	//m_pManager_Render = pWorld->SpawnActor<AHHM_Manager_Render>();
	//m_pManager_LocalMap = pWorld->SpawnActor<AHHM_Manager_LocalMap>();
	//m_pManager_Navigation = pWorld->SpawnActor<AHHM_Manager_Navigation>();

	//m_pManager_Debug = pWorld->SpawnActor<AHHM_Manager_Debug>();



	pWorld->SpawnActor<ATouchPanel>();
	

	/*m_pManager_Render = NewObject<AHHM_Manager_Render>(this, TEXT("Manager_Render"));
	bool b = m_pManager_Render->HasActorBegunPlay();
	m_pManager_LocalMap = NewObject<AHHM_Manager_LocalMap>(this, TEXT("Manager_LocalMap"));*/
}