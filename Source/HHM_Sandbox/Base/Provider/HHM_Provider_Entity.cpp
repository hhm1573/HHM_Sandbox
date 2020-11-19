// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Provider_Entity.h"

#include "Engine/World.h"

#include "Entity/HHM_Entity.h"
#include "Entity/Walkable/HHM_Entity_Walkable.h"



UHHM_Provider_Entity::UHHM_Provider_Entity() {
	//Register Entity here
	m_Container_Registered_Entity.Add(0, AHHM_Entity_Walkable::StaticClass());

}

AHHM_Entity* UHHM_Provider_Entity::Spawn_Entity_By_ID(int32 _id, FVector& _location)
{
	bool IsValid_ID = m_Container_Registered_Entity.Contains(_id);
	if (IsValid_ID == false) {
		//Exception Caution Not Valid Entity ID
		return nullptr;
	}



	//Create Actor From Input ID
	UWorld* pWorld = nullptr;
	pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception Danger Can't get World
		return nullptr;
	}

	UClass* pClass_Create = m_Container_Registered_Entity[_id];

	AHHM_Entity* pEntity_Created = nullptr;
	FActorSpawnParameters SpawnParam = FActorSpawnParameters();
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	pEntity_Created = pWorld->SpawnActor<AHHM_Entity>(pClass_Create, _location, FRotator::ZeroRotator, SpawnParam);
	if (pEntity_Created == nullptr) {
		//Exception Danger Can't Spawn Actor
		return nullptr;
	}

	return pEntity_Created;
}

bool UHHM_Provider_Entity::Register_Entity(int32 _id_ToRegist, UClass* _pClass)
{
	bool IsIdExist = m_Container_Registered_Entity.Contains(_id_ToRegist);
	if (IsIdExist == true) {
		//Exception Caution Id Not Valid. Entity Not registered.
		return false;
	}

	m_Container_Registered_Entity.Add(_id_ToRegist, _pClass);

	return true;
}
