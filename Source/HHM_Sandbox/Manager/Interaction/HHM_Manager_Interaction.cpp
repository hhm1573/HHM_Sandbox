// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Manager_Interaction.h"



AHHM_Manager_Interaction::AHHM_Manager_Interaction() {
	m_Container_InteractionBaseData.Empty();
}



void AHHM_Manager_Interaction::BeginPlay()
{
	Super::BeginPlay();

	Register_AllInteractionBaseData();

	return;
}

void AHHM_Manager_Interaction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	return;
}



bool AHHM_Manager_Interaction::Get_BaseData_Interaction(const FHHM_Data_Interaction_Base*& _pBaseData_Interaction_Return, int32 _id_Interaction)
{
	if (_id_Interaction < 0) {
		//Exception negative id value
		return false;
	}

	FHHM_Data_Interaction_Base* pBaseData_Found = nullptr;
	pBaseData_Found = m_Container_InteractionBaseData.Find(_id_Interaction);

	if (pBaseData_Found == nullptr) {
		//Exception Invalid interaction id
		return false;
	}

	_pBaseData_Interaction_Return = pBaseData_Found;
	return true;
}



void AHHM_Manager_Interaction::Register_AllInteractionBaseData()
{
	//Debug Interact to tile
	m_Container_InteractionBaseData.Add(HHM_INTERACTION_ID_DEBUG_TO_TILE, FHHM_Data_Interaction_Base());
	/*m_Container_InteractionBaseData[HHM_INTERACTION_ID_DEBUG_TO_TILE].m_ID = HHM_INTERACTION_ID_DEBUG_TO_TILE;
	m_Container_InteractionBaseData[HHM_INTERACTION_ID_DEBUG_TO_TILE].m_Name = HHM_INTERACTION_NAME_DEBUG_TO_TILE;
	m_Container_InteractionBaseData[HHM_INTERACTION_ID_DEBUG_TO_TILE].m_Type = EHHM_InteractionType::Type_Interaction_SelfToTile;*/
	m_Container_InteractionBaseData[HHM_INTERACTION_ID_DEBUG_TO_TILE].Set_BaseData(HHM_INTERACTION_ID_DEBUG_TO_TILE, HHM_INTERACTION_NAME_DEBUG_TO_TILE
		, EHHM_InteractionType::Type_Interaction_SelfToTile, 5.0f);
}

void AHHM_Manager_Interaction::Execute_Interact(FHHM_Data_Interaction& _data_Interaction, UHHM_Component_InteractionHandler* _pInteractionHandler_Executer)
{
}
