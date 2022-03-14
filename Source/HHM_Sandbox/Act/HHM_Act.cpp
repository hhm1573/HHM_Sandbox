// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Act.h"



UHHM_Act::UHHM_Act() {

}

bool UHHM_Act::Begin_Act()
{
	//Exception This function should not be called.
	return false;
}

void UHHM_Act::Performed_Action(const bool& _isActEnd_Return)
{
	//Exception This function should not be called.
	//this function is pure function.
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
