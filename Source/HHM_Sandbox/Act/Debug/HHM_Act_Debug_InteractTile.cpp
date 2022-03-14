// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Act_Debug_InteractTile.h"

#include "Engine/World.h"
#include "Base/GameMode/HHM_GameMode_LocalMap.h"
#include "Manager/Interaction/HHM_Manager_Interaction.h"

#include "Data/LocalMap/LocalMap.h"
#include "Entity/HHM_Entity.h"
#include "Manager/Math/Grid/HHM_Manager_Math_Grid.h"



UHHM_Act_Debug_InteractTile::UHHM_Act_Debug_InteractTile() {

}



bool UHHM_Act_Debug_InteractTile::Begin_Act()
{
	UWorld* pWorld = nullptr;														//Act에 Get Manager 함수 만들어서 해당부분 축소. 타일위치 구하는 부분도 Act로 이동.
	pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception Getting world failed
		return false;
	}

	AGameModeBase* pGameMode_Raw = nullptr;
	pGameMode_Raw = pWorld->GetAuthGameMode();
	if (pGameMode_Raw == nullptr) {
		//Exception getting authgamemode failed
		return false;
	}

	AHHM_GameMode_LocalMap* pGameMode = nullptr;
	pGameMode = Cast<AHHM_GameMode_LocalMap>(pGameMode_Raw);
	if (pGameMode == nullptr) {
		//Exception casting raw gamemode into GameMode_LocalMap failed
		return false;
	}

	AHHM_Manager_Interaction* pManager_Interaction = nullptr;
	pManager_Interaction = pGameMode->Get_Manager_Interaction();
	if (pManager_Interaction == nullptr) {
		//Exception getting interaction manager failed
		return false;
	}



	const FHHM_Data_Interaction_Base* pBaseData_Interact_Debug_To_Tile = nullptr;
	bool IsSucceed_GetBaseData_InteractToTile = pManager_Interaction->Get_BaseData_Interaction(pBaseData_Interact_Debug_To_Tile, HHM_INTERACTION_ID_DEBUG_TO_TILE);
	if (IsSucceed_GetBaseData_InteractToTile == false || pBaseData_Interact_Debug_To_Tile == nullptr) {
		//Exception Getting base data failed
		return false;
	}



	FHHM_Data_Queue_Interaction InteractionQueue;
	InteractionQueue.m_pInteractor = m_pInteractor;

	InteractionQueue.m_Queue_Interaction.Add(FHHM_Data_Interaction());
	InteractionQueue.m_Queue_Interaction[0].m_BaseData_Interaction = *pBaseData_Interact_Debug_To_Tile;
	InteractionQueue.m_Queue_Interaction[0].m_Target_Interaction_Location = m_Target_Location;
	InteractionQueue.m_Queue_Interaction[0].m_Timer_Interaction = pBaseData_Interact_Debug_To_Tile->m_Time;
	InteractionQueue.m_Queue_Interaction[0].m_Timer_EndWait = InteractionQueue.m_Queue_Interaction[0].m_Timer_Interaction * 2.0f;



	ALocalMap* pLocalMap = m_pInteractor->Get_LocalMap();
	if (pLocalMap == nullptr) {
		//Exception cant get local map
		return false;
	}
	FHHM_MapInfo MapInfo = pLocalMap->Get_MapInfo_ConstRef();
	FVector TileLocation = FVector(-1.0f);
	bool IsSucceed_GetTileLocation = AHHM_Manager_Math_Grid::Convert_IndexLocation_To_Translation(TileLocation, FVector2D(m_Target_Location.X, m_Target_Location.Z), MapInfo);
	if (IsSucceed_GetTileLocation == false) {
		//Exception converting tile location failed
		return false;
	}



	FHHM_Data_Action Action;
	Action.m_Queue_Interaction = InteractionQueue;
	Action.m_Location_Target = TileLocation;
	Action.m_AcceptableRadius = 10.0f;

	m_CurrentAction = Action;

	return true;
}
