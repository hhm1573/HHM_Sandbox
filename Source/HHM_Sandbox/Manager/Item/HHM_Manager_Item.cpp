// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Manager_Item.h"

#include "Engine/World.h"

#include "UObject/UObjectIterator.h"
#include "AssetRegistryModule.h"

#include "Item/Base/HHM_Item.h"



const FString AHHM_Manager_Item::ItemName_Base(TEXT("Item_Base"));
const FString AHHM_Manager_Item::ItemName_Throwable(TEXT("Item_Throwable"));



AHHM_Manager_Item::AHHM_Manager_Item() {

}



void AHHM_Manager_Item::BeginPlay()
{
	Super::BeginPlay();

	Register_All_Item();
	Register_All_ItemData();
}



UHHM_ItemData* AHHM_Manager_Item::BP_Create_Default_ItemData_By_ID(int32 _id, int32 _subID)
{
	return Create_Default_ItemData_By_ID(_id, _subID);
}



const UHHM_Item* AHHM_Manager_Item::Get_Item_By_Name(FString _name) const
{
	bool IsRegisteredName = m_Container_Item.Contains(_name);
	if (IsRegisteredName == true) {
		return m_Container_Item[_name];
	}

	//Exception Invalid item name
	return nullptr;
}

const UHHM_ItemData* AHHM_Manager_Item::Get_ItemData_By_ID(int32 _id, int32 _subID) const
{
	bool IsRegisteredID = m_Container_ItemData.Contains(_id);
	if (IsRegisteredID == true) {
		bool IsRegisteredSubID = m_Container_ItemData[_id].Container_ItemData.Contains(_subID);
		if (IsRegisteredSubID == true) {
			return m_Container_ItemData[_id].Container_ItemData[_subID];
		}
	}

	//Exception invalid itemdata id
	return nullptr;
}

UHHM_ItemData* AHHM_Manager_Item::Create_Default_ItemData_By_ID(int32 _id, int32 _subID)
{
	const UHHM_ItemData* pItemData_Default = nullptr;
	pItemData_Default = Get_ItemData_By_ID(_id, _subID);
	if (pItemData_Default == nullptr) {
		//Exception Invalid item id
		return nullptr;
	}

	UClass* pClass = pItemData_Default->GetClass();
	UHHM_ItemData* pItemData_Created = NewObject<UHHM_ItemData>(this, pClass);
	if (pItemData_Created == nullptr) {
		//Exception create item failed
		return nullptr;
	}

	pItemData_Created->Set_Item(pItemData_Default->Get_Item());

	return pItemData_Created;
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

			FString ItemName = pItem->Get_ItemName();

			const bool IsRegisteredName = m_Container_Item.Contains(ItemName);

			if (IsRegisteredName == true) {
				//Exception overlapping item name. [ClassName] is not registered due to overlapping name.
				continue;
			}

			m_Container_Item.Add(ItemName, pItem);
		}
	}

	return;
}

void AHHM_Manager_Item::Register_All_ItemData(void)
{
	UWorld* pWorld = nullptr;
	pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception
		return;
	}

	for (TObjectIterator<UClass> Iterator; Iterator; ++Iterator) {
		if (Iterator->IsChildOf(UHHM_ItemData::StaticClass()) && !Iterator->HasAnyClassFlags(EClassFlags::CLASS_Abstract)) {
			UHHM_ItemData* pItemData = nullptr;
			UClass* pClass = *Iterator;
			FName Name_Class = pClass->GetFName();
			FString Name_Instance = Name_Class.ToString();
			Name_Instance.Append(TEXT("_HHM"));
			pItemData = NewObject<UHHM_ItemData>(this, pClass, *Name_Instance);

			int32 ItemData_ID = pItemData->Get_ID();
			int32 ItemData_SubID = pItemData->Get_SubID();

			const bool IsRegisteredID = m_Container_ItemData.Contains(ItemData_ID);
			if (IsRegisteredID == false) {
				m_Container_ItemData.Add(ItemData_ID, FHHM_Container_ItemData());
			}

			const bool IsRegisteredSubID = m_Container_ItemData[ItemData_ID].Container_ItemData.Contains(ItemData_SubID);
			if (IsRegisteredSubID == true) {
				//Exception Critical ItemData SubID Already taken
				continue;
			}



			//Set Item reference for ItemData
			FString ItemName = pItemData->Get_ItemName();
			bool IsValidName = m_Container_Item.Contains(ItemName);
			if (IsValidName == false) {
				//Excedption Critical Invalid ItemName
				continue;
			}
			pItemData->Set_Item(m_Container_Item[ItemName]);



			m_Container_ItemData[ItemData_ID].Container_ItemData.Add(ItemData_SubID, pItemData);
		}
	}

	return;
}
