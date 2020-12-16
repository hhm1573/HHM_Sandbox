// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Entity_Walkable.h"

#include "Base/Component/Movement/HHM_Component_Movement.h"



AHHM_Entity_Walkable::AHHM_Entity_Walkable() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	if (m_pComponent_Movement == nullptr) {
		m_pComponent_Movement = CreateDefaultSubobject<UHHM_Component_Movement>(TEXT("HHM_MovementComponent"));
	}
	m_pComponent_Movement->UpdatedComponent = RootComponent;



	// HHM Note To Me : Fix this later if has better idea. 
	// Delete this later. when you change variables of component in constructor, it will be wiped out and set as zero for some reason.
	FHHM_Entity_MovementData MoveData = FHHM_Entity_MovementData();
	MoveData.Jump_Vertical_MaxHeight = 3;
	MoveData.DownJump_MaxHeight = 3;
	MoveData.Jump_Horizontal_MaxLength = 3;
	MoveData.Jump_Vertical_FreeJump_Minimum = 10;
	MoveData.Jump_Vertical_AnimationDuration.Add(1.5f);
	MoveData.Jump_Vertical_AnimationDuration.Add(2.0f);
	m_pComponent_Movement->Temp_Set_MoveData(MoveData);

	m_pComponent_Movement->Temp_Set_DefaultSpeed(1.0f);
}



void AHHM_Entity_Walkable::BeginPlay()
{
	Super::BeginPlay();

	// HHM Note To Me : Fix this later if has better idea.
	FHHM_Entity_MovementData MoveData = FHHM_Entity_MovementData();
	MoveData.Jump_Vertical_MaxHeight = 3;
	MoveData.DownJump_MaxHeight = 3;
	MoveData.Jump_Horizontal_MaxLength = 3;
	MoveData.Jump_Vertical_FreeJump_Minimum = 10;
	MoveData.Jump_Vertical_AnimationDuration.Add(2.0f);
	MoveData.Jump_Vertical_AnimationDuration.Add(2.5f);
	MoveData.DownJump_FreeFall_Minimum = 10;
	MoveData.DownJump_AnimationDuration.Add(1.0f);
	MoveData.DownJump_AnimationDuration.Add(1.5f);
	m_pComponent_Movement->Temp_Set_MoveData(MoveData);

	m_pComponent_Movement->Temp_Set_DefaultSpeed(1.0f);
}

bool AHHM_Entity_Walkable::Order_MoveToLocation(int32 _index_Horizontal, int32 _index_Vertical) {
	if (m_pComponent_Movement == nullptr) {
		//Exception
		return false;
	}

	bool IsSucceed_Order = m_pComponent_Movement->MoveToLocation(_index_Horizontal, _index_Vertical);
	return IsSucceed_Order;
}



float AHHM_Entity_Walkable::Get_MoveTimer() {
	if (m_pComponent_Movement == nullptr) {
		//Exception
		return -1.0f;
	}

	return m_pComponent_Movement->Get_MoveTimer();
}

UHHM_Component_Movement* AHHM_Entity_Walkable::Get_Component_Movement()
{
	if (m_pComponent_Movement == nullptr) {
		//Exception
	}
	return m_pComponent_Movement;
}
