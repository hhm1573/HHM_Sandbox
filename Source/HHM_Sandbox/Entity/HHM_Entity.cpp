// Fill out your copyright notice in the Description page of Project Settings.

#include "HHM_Entity.h"

#include "Base/Component/Movement/HHM_Component_Movement.h"

#include "Base/GameInstance/HHM_GameInstance.h"

#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Base/GameMode/HHM_GameMode_LocalMap.h"
#include "Manager/LocalMap/HHM_Manager_LocalMap.h"

#include "Data/LocalMap/LocalMap.h"
//#include "Manager/LocalMap/HHM_Manager_LocalMap.h"

// Sets default values
AHHM_Entity::AHHM_Entity()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHHM_Entity::BeginPlay()
{
	Super::BeginPlay();
	
	//Register Entity. if register failed, destroy by self
	if (m_EntityID < 0) {
		bool IsSucceed_Register = Register_Entity();
		if (IsSucceed_Register == false) {
			//Exception failed register
			bool IsSucceed_Destroy = Destroy();
			if (IsSucceed_Destroy == false) {
				//Exception failed Destroy
			}
			return;
		}

		if (m_EntityID < 0 || m_pLocalMap == nullptr) {
			//Exception Not Successfuly registered
			Destroy();
			return;
		}
	}
	
	
}

// Called every frame
void AHHM_Entity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHHM_Entity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHHM_Entity::BeginDestroy() {
	Super::BeginDestroy();

	if (m_EntityID >= 0 || m_pLocalMap != nullptr) {
		DeRegister_Entity();
	}
}



bool AHHM_Entity::Register_Entity(void)
{
	/* if (_pLocalMap == nullptr) {
		//Exception Input LocalMap is nullptr
		bool IsDestroySucceed = Destroy();
		if (IsDestroySucceed == false) {
			//Exception Can't destroy actor
		}
		return;
	}

	if (_entityID < 0) {
		bool IsDestroySucceed = Destroy();
		if (IsDestroySucceed == false) {
			//Exception Can't destroy actor
		}
		return;
	}

	m_pLocalMap = _pLocalMap;
	m_EntityID = _entityID; */

	//Work here

	UWorld* pWorld = nullptr;
	pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception
		return false;
	}

	AGameModeBase* pGameMode_Raw = nullptr;
	pGameMode_Raw = pWorld->GetAuthGameMode();
	if (pGameMode_Raw == nullptr) {
		//Exception
		return false;
	}

	AHHM_GameMode_LocalMap* pGameMode = nullptr;
	pGameMode = Cast<AHHM_GameMode_LocalMap>(pGameMode_Raw);
	if (pGameMode == nullptr) {
		//Exception
		return false;
	}

	FVector ActorLocation = GetActorLocation();

	bool IsSucceed_Register = pGameMode->Register_Entity(this, &m_EntityID, &m_pLocalMap, ActorLocation);
	if (IsSucceed_Register == false) {
		//Exception
		return false;
	}

	return true;
}

void AHHM_Entity::DeRegister_Entity(void) {
	
	m_pLocalMap->Entity_Deregister(this);

	m_pLocalMap = nullptr;
	m_EntityID = -1;

	bool IsDestroySucceed = Destroy();
	if (IsDestroySucceed == false) {
		//Exception Can't destroy actor
	}
	return;
}



void AHHM_Entity::Interaction_Begin()
{
}

void AHHM_Entity::Interaction_End()
{
}

void AHHM_Entity::Action_End(FHHM_Data_Queue_Interaction& _action)
{
}

void AHHM_Entity::Action_Canceled(FHHM_Data_Queue_Interaction& _action)
{
}



void AHHM_Entity::HHM_Jump_Implementation(const FVector& jump_LocationStart, const FVector& jump_LocationEnd, const int32& jump_Height) {

	if (jump_Height == 0) {
		//Exception
		return;
	}

	float Jump_Duration = 2.0f;

	float Velocity_X = (jump_LocationEnd.X - jump_LocationStart.X) / Jump_Duration;
	float Velocity_Y = (jump_LocationEnd.Y - jump_LocationStart.Y) / Jump_Duration;
	float Velocity_Z = (jump_LocationEnd.Z + 0.5f * 981.0f * Jump_Duration * Jump_Duration - jump_LocationEnd.Z) / Jump_Duration;
	FVector Velocity = FVector(Velocity_X, Velocity_Y, Velocity_Z);

	LaunchCharacter(Velocity, true, true);

	/*FVector	StartToEnd = jump_LocationEnd - jump_LocationStart;

	float Velocity_X = StartToEnd.X / Jump_Duration;
	float Velocity_Y = StartToEnd.Y / Jump_Duration;*/
	

}

void AHHM_Entity::HHM_FallOff_Implementation(const FVector& fall_LocationStart, const FVector& fall_LocationEnd, const int32& fall_Height) {

}

void AHHM_Entity::HHM_HorizontalJump_Implementation(const FVector& _horizontalJump_LocationStart, const FVector& _horizontalJump_LocationEnd, const int32 _jump_Length)
{
}



bool AHHM_Entity::Attack_Tile(int32 index_Horizontal, int32 index_Vertical, int32 damage) {
	if (m_pLocalMap == nullptr) {
		//Exception No LocalMap Found. Entity Might Not registered.
		if (Destroy() == false) {
			//Exception Cant destroy actor
		}
		return false;
	}

	return m_pLocalMap->Damage_Tile(damage, EHHM_DamageType::Damage_Melee, this, index_Horizontal, index_Vertical);
}

bool AHHM_Entity::Place_Tile(int32 index_Horizontal, int32 index_Vertical, int32 tileID, int32 tileSubID) {	
	//temporary function. it will replaced with Use_Item or place_Item thing and there will be place function on local map class
	if (m_pLocalMap == nullptr) {
		//Exception No LocalMap Found. Entity Might Not registered.
		if (Destroy() == false) {
			//Exception Cant destroy actor
		}
		return false;
	}

	const bool IsLocation_Placeable = m_pLocalMap->Check_Location_TilePlaceable(index_Horizontal, index_Vertical);
	if (IsLocation_Placeable == false) {
		return false;
	}

	m_pLocalMap->Set_Tile_At_Pos(index_Horizontal, index_Vertical, tileID, tileSubID);
	return true;
}