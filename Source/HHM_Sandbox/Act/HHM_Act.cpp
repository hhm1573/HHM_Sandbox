// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Act.h"

#include "Engine/World.h"
#include "Base/GameMode/HHM_GameMode_LocalMap.h"



UHHM_Act::UHHM_Act() {

}

void UHHM_Act::BP_Set_Target(AHHM_Entity* _pInteractor, AHHM_Entity* _pTarget_Entity, FVector _targetLocation)
{
	Set_Target(_pInteractor, _pTarget_Entity, _targetLocation);
}

bool UHHM_Act::Begin_Act()
{
	//Exception This function should not be called.
	return false;
}

bool UHHM_Act::Performed_Action()
{
	++m_CurrentActionIndex;
	return true;
}

void UHHM_Act::On_Cancel()
{
	m_Canceled = true;
}

void UHHM_Act::Set_Target(AHHM_Entity* _pInteractor, AHHM_Entity* _pTarget_Entity, FVector _targetLocation)
{
	m_pInteractor = _pInteractor;

	m_pTarget_Entity = _pTarget_Entity;
	m_Target_Location = _targetLocation;

	return;
}

AHHM_Manager_Interaction* UHHM_Act::Get_Manager_Interaction()
{
	UWorld* pWorld = nullptr;
	pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception
		return nullptr;
	}

	AGameModeBase* pGameMode_Raw = nullptr;
	pGameMode_Raw = pWorld->GetAuthGameMode();
	if (pGameMode_Raw == nullptr) {
		//Exception
		return nullptr;
	}

	AHHM_GameMode_LocalMap* pGameMode = nullptr;
	pGameMode = Cast<AHHM_GameMode_LocalMap>(pGameMode_Raw);
	if (pGameMode == nullptr) {
		//Exception
		return nullptr;
	}

	AHHM_Manager_Interaction* pManager_Interaction = nullptr;
	pManager_Interaction = pGameMode->Get_Manager_Interaction();
	return pManager_Interaction;
}
