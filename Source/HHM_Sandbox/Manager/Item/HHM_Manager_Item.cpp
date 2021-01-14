// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Manager_Item.h"

#include "Engine/World.h"

#include "UObject/UObjectIterator.h"
#include "AssetRegistryModule.h"

#include "Data/Item/HHM_Item.h"



AHHM_Manager_Item::AHHM_Manager_Item() {

}



void AHHM_Manager_Item::BeginPlay()
{
	Super::BeginPlay();

	Register_All_Item();
}

UHHM_Item* AHHM_Manager_Item::Get_Item_By_ID(int32 _id)
{
	bool IsRegisteredID = m_Container_Item.Contains(_id);
	if (IsRegisteredID == true) {
		return m_Container_Item[_id];
	}

	return nullptr;
}

const UHHM_Item* AHHM_Manager_Item::Get_Item_By_ID_Const(int32 _id) const
{
	bool IsRegisteredID = m_Container_Item.Contains(_id);
	if (IsRegisteredID == true) {
		return m_Container_Item[_id];
	}

	return nullptr;
}

const UHHM_ItemData* AHHM_Manager_Item::Get_DefaultItemData_By_ID(int32 _id) const
{
	bool IsRegisteredID = m_Container_Item.Contains(_id);
	if (IsRegisteredID == true) {
		return m_Container_Item[_id]->Get_DefaultItemData();
	}

	return m_Container_Item[_id]->Get_DefaultItemData();	// HHM Note : make it return ZeroData Later like zerovector
}

void AHHM_Manager_Item::Register_All_Item(void)
{
	UWorld* pWorld = nullptr;
	pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception
		return;
	}

	for (TObjectIterator<UClass> Iterator; Iterator; ++Iterator) {
		if (Iterator->IsChildOf(UHHM_Item::StaticClass()) && !Iterator->HasAnyClassFlags(EClassFlags::CLASS_Abstract)) {
			UHHM_Item* pItem = nullptr;
			UClass* pClass = *Iterator;
			pItem = NewObject<UHHM_Item>(this, pClass);

			int32 ItemID = pItem->Get_ItemID();
			const bool IsRegisteredID = m_Container_Item.Contains(ItemID);
			if (IsRegisteredID == true) {
				//Exception Critical
				return;
			}

			m_Container_Item.Add(ItemID, pItem);
		}
	}

	return;
}
