// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Act_Debug_GetItemFromTile.h"

#include "Entity/HHM_Entity.h"



UHHM_Act_Debug_GetItemFromTile::UHHM_Act_Debug_GetItemFromTile() {

}



bool UHHM_Act_Debug_GetItemFromTile::Begin_Act()
{
	AHHM_Manager_Interaction* pManager_Interaction = Get_Manager_Interaction();
	if (pManager_Interaction == nullptr) {
		//Exception
		return false;
	}

	const FHHM_Data_Interaction_Base* pBaseData_Interact_Debug_Get_Item_From_Tile = nullptr;
	bool IsSucceed_GetBaseData_Interact_GetItemFromTile = pManager_Interaction->Get_BaseData_Interaction(pBaseData_Interact_Debug_Get_Item_From_Tile, HHM_INTERACTION_ID_DEBUG_GET_ITEM_FROM_TILE);
	if (IsSucceed_GetBaseData_Interact_GetItemFromTile == false) {
		//Exception
		return false;
	}



	//Build Interaction Queue
	FHHM_Data_Queue_Interaction InteractionQueue = FHHM_Data_Queue_Interaction();
	InteractionQueue.m_pInteractor = m_pInteractor;

	InteractionQueue.m_Queue_Interaction.Add(FHHM_Data_Interaction());
	InteractionQueue.m_Queue_Interaction[0].m_BaseData_Interaction = *pBaseData_Interact_Debug_Get_Item_From_Tile;
	InteractionQueue.m_Queue_Interaction[0].m_Target_Interaction_Location = m_Target_Location;
	InteractionQueue.m_Queue_Interaction[0].m_Timer_Interaction = pBaseData_Interact_Debug_Get_Item_From_Tile->m_Time;
	InteractionQueue.m_Queue_Interaction[0].m_Timer_EndWait = InteractionQueue.m_Queue_Interaction[0].m_Timer_Interaction * 2.0f;



	//Build Action
	FHHM_Data_Action Action = FHHM_Data_Action();
	Action.m_Queue_Interaction = InteractionQueue;
	Action.m_Location_Target = FVector(m_Target_Location.X, 0.0f, m_Target_Location.Z);
	Action.m_AcceptableRadius = 5.0f;
	m_CurrentAction = Action;
	
	return true;
}
