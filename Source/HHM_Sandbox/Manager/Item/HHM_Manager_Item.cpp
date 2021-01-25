// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Manager_Item.h"

#include "Engine/World.h"

#include "UObject/UObjectIterator.h"
#include "AssetRegistryModule.h"

#include "Item/Base/HHM_Item.h"



AHHM_Manager_Item::AHHM_Manager_Item() {

}



void AHHM_Manager_Item::BeginPlay()
{
	Super::BeginPlay();

	Register_All_Item();
}

UHHM_Item* AHHM_Manager_Item::Get_Item_By_ID(int32 _id, int32 _subID)
{
	bool IsRegisteredID = m_Container_Item.Contains(_id);
	if (IsRegisteredID == true) {
		bool IsRegisteredSubID = m_Container_Item[_id].Container_SubItem.Contains(_subID);
		if (IsRegisteredSubID == true) {
			return m_Container_Item[_id].Container_SubItem[_subID];
		}
	}

	return nullptr;
}

const UHHM_Item* AHHM_Manager_Item::Get_Item_By_ID_Const(int32 _id, int32 _subID) const
{
	bool IsRegisteredID = m_Container_Item.Contains(_id);
	if (IsRegisteredID == true) {
		bool IsRegisteredSubID = m_Container_Item[_id].Container_SubItem.Contains(_subID);
		if (IsRegisteredSubID == true) {
			return m_Container_Item[_id].Container_SubItem[_subID];
		}
	}

	return nullptr;
}

const UHHM_ItemData* AHHM_Manager_Item::Get_DefaultItemData_By_ID(int32 _id, int32 _subID) const
{
	bool IsRegisteredID = m_Container_Item.Contains(_id);
	if (IsRegisteredID == true) {
		bool IsRegisteredSubID = m_Container_Item[_id].Container_SubItem.Contains(_subID);
		if (IsRegisteredSubID == true) {
			return m_Container_Item[_id].Container_SubItem[_subID]->Get_DefaultItemData();
		}
	}

	return m_Container_Item[_id].Container_SubItem[_subID]->Get_DefaultItemData();	// HHM Note : make it return ZeroData Later like zerovector
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
			FName Name_Class = pClass->GetFName();
			FString Name_Instance = Name_Class.ToString();
			Name_Instance.Append(TEXT("_HHM"));
			pItem = NewObject<UHHM_Item>(this, pClass, *Name_Instance);

			int32 ItemID = pItem->Get_ItemID();
			int32 ItemSubID = pItem->Get_ItemSubID();

			const bool IsRegisteredID = m_Container_Item.Contains(ItemID);
			if (IsRegisteredID == false) {
				m_Container_Item.Add(ItemID, FHHM_Container_SubItem());
			}

			const bool IsRegisteredSubID = m_Container_Item[ItemID].Container_SubItem.Contains(ItemSubID);
			if (IsRegisteredSubID == true) {
				//Exception Critical
				return;
			}
			m_Container_Item[ItemID].Container_SubItem.Add(ItemSubID, pItem);
		}
	}

	return;
}
