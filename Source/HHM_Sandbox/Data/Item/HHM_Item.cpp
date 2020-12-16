// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Item.h"



UHHM_Item::UHHM_Item(void) {
	m_DefaultItemData = FHHM_ItemData(this, 0, 0);
}

bool UHHM_Item::On_ItemUsed(ALocalMap* _pLocalMap, AEntity* _pUser, FVector _targetLocation, AEntity* _pTarget)
{
	return false;
}
