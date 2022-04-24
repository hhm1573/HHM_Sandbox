// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Entity_Walkable.h"

#include "Base/Component/Movement/HHM_Component_Movement.h"

#include "AIModule/Classes/AIController.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"

#include "Manager/Math/Grid/HHM_Manager_Math_Grid.h"
#include "Data/LocalMap/LocalMap.h"

#include "Act/HHM_Act.h"
#include "Header/Macro.h"



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
	MoveData.Jump_Vertical_AnimationDuration.Add(3.0f);
	MoveData.DownJump_FreeFall_Minimum = 10;
	MoveData.DownJump_AnimationDuration.Add(1.0f);
	MoveData.DownJump_AnimationDuration.Add(1.5f);
	MoveData.DownJump_AnimationDuration.Add(2.0f);
	m_pComponent_Movement->Temp_Set_MoveData(MoveData);

	m_pComponent_Movement->Temp_Set_DefaultSpeed(1.0f);
}

void AHHM_Entity_Walkable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Act_Tick(DeltaTime);
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



#pragma region Act



bool AHHM_Entity_Walkable::BP_Action_Perform(UHHM_Act*& _act_Return, int32& _actionIndex_Return)
{
	if (m_pLocalMap == nullptr) {
		//Exception
		return false;
	}
	const FHHM_MapInfo& MapInfo = m_pLocalMap->Get_MapInfo_ConstRef();

	if (m_pComponent_InteractionHandler == nullptr) {
		//Exception
		return false;
	}

	if (m_Act_Performing == nullptr) {
		//Exception
		return false;
	}



	FHHM_Data_Action& Action = m_Act_Performing->Get_Action();
	if (Action.m_Location_Target == FVector::ZeroVector) {		//Action Not Set
		//Exception
		return false;
	}

	FVector ActionLocation = FVector::ZeroVector;
	bool IsSucceed_ConvertLocation = AHHM_Manager_Math_Grid::Convert_IndexLocation_To_Translation(ActionLocation, FVector2D(Action.m_Location_Target.X, Action.m_Location_Target.Z), MapInfo, true);
	if (IsSucceed_ConvertLocation == false) {
		//Exception
		return false;
	}// To Do : 타일 위치 높이 바닥으로 내리고 Y좌표 삭제후 거리 구하기. BP Task의 valid check 부분에서 거리연산 삭제하고 Finish execute를 finish task로 대체

	//Get Actor Location
	FVector CurrentLocation = GetActorLocation();



	//Adjust TargetTileLocation and ActorLocation.
	FVector Adjusted_ActionLocation = ActionLocation;
	Adjusted_ActionLocation.Y = 0.0f;
	Adjusted_ActionLocation.Z = Adjusted_ActionLocation.Z - (HHM_TILE_SIZE * 0.5f);
	FVector Adjusted_ActorLocation = CurrentLocation;
	Adjusted_ActorLocation.Y = 0.0f;



	float DistanceToTarget = FVector::Distance(Adjusted_ActorLocation, Adjusted_ActionLocation);
	if (DistanceToTarget > Action.m_AcceptableRadius) {
		//Exception
		return false;
	}





	//Add Action's interaction queue to interaction handler component.
	bool IsFocussed = false;
	float EstimatedWaitTime = -1.0f;
	int32 RegisteredID = -1;
	const bool IsSucceed_AddAction = m_pComponent_InteractionHandler->Action_Add(IsFocussed, EstimatedWaitTime, RegisteredID, Action.m_Queue_Interaction);
	if (IsSucceed_AddAction == false) {
		return false;
	}

	//Set Return values
	_act_Return = m_Act_Performing;
	_actionIndex_Return = m_Act_Performing->Get_CurrentActionIndex();

	return true;
}

bool AHHM_Entity_Walkable::BP_Action_Tick(const UHHM_Act* _act_Last, int32 _actionIndex_Last)
{
	if (_act_Last == nullptr) {
		//Exception
		return true;
	}

	if (m_Act_Performing != _act_Last) {	//Action Finished
		return true;
	}

	if (m_Act_Performing != nullptr) {
		int32 CurrentActionIndex = m_Act_Performing->Get_CurrentActionIndex();
		if (CurrentActionIndex != _actionIndex_Last) {
			return true;
		}
	}

	return false;
}

void AHHM_Entity_Walkable::BP_Debug_Act_Queue_Add(UHHM_Act* _pAct)
{
	Act_Queue_Add(_pAct);
}

void AHHM_Entity_Walkable::Action_End(FHHM_Data_Queue_Interaction& _action)
{
	bool IsActEnd = m_Act_Performing->Performed_Action();
	if (IsActEnd == true) {
		m_Act_Performing = nullptr;
	}
}

void AHHM_Entity_Walkable::Action_Canceled(FHHM_Data_Queue_Interaction& _action)
{
}



void AHHM_Entity_Walkable::Act_Start(UHHM_Act* _pAct)
{
	if (_pAct == nullptr) {
		//Exception
		return;
	}

	Act_Queue_Clear();
	Act_Cancel();
	Act_Queue_Add(_pAct);
}

void AHHM_Entity_Walkable::Act_Cancel()
{
	if (m_Act_Performing == nullptr) {
		return;
	}

	int32 InteractionRegisteredID = m_Act_Performing->Get_Action().m_InteractionID;

	m_pComponent_InteractionHandler->Action_Cancel(InteractionRegisteredID);
	m_Act_Performing->On_Cancel();
}

void AHHM_Entity_Walkable::Act_Queue_Add(UHHM_Act* _pAct)
{
	if (_pAct == nullptr) {
		//Exception
		return;
	}

	m_Queue_Act.Add(_pAct);
}

void AHHM_Entity_Walkable::Act_Queue_Clear()
{
	m_Queue_Act.Empty();
}



void AHHM_Entity_Walkable::Act_Tick(float DeltaTime)
{
	//실행중인 act가 없을경우 큐 체크
	if (m_Act_Performing == nullptr) {
		int32 Num_Act_Queued = m_Queue_Act.Num();
		if (Num_Act_Queued > 0) {
			//m_Act_Performing = m_Queue_Act[0];
			Act_Begin(m_Queue_Act[0]);
			m_Queue_Act.RemoveAt(0);
		}
		else {
			//There is no queued act.
			return;
		}
	}



	//act 타겟 위치로 가서 상호작용핸들러 작동

}

void AHHM_Entity_Walkable::Act_Begin(UHHM_Act* _pAct)
{
	if (_pAct == nullptr) {
		//Exception
		return;
	}

	bool IsSucceed_BeginAct = _pAct->Begin_Act();
	if (IsSucceed_BeginAct == false) {
		//Exception
		return;
	}

	m_Act_Performing = _pAct;



	//AAIController* pAIController = nullptr;
	//pAIController = GetController<AAIController>();
	//if (pAIController == nullptr) {
	//	//Exception
	//	return;
	//}
	//
	//UBlackboardComponent* pBlackBoard = nullptr;
	//pBlackBoard = pAIController->GetBlackboardComponent();
	//if (pBlackBoard == nullptr) {
	//	//Exception
	//	return;
	//}
	//
	//FHHM_Data_Action& Action = m_Act_Performing->Get_Action();
	//pBlackBoard->SetValueAsVector(FName(TEXT("TargetMoveLocation")), Action.m_Location_Target);
	//pBlackBoard->SetValueAsFloat(FName(TEXT("MoveAcceptableDistance")), Action.m_AcceptableRadius);
	//pBlackBoard->SetValueAsBool(FName(TEXT("HasActionToPerform")), true);

	//bool IsActionFocused = false;
	//float EstimatedWaitTime = -1.0f;
	//int32 IDRegistered = -1;
	//m_pComponent_InteractionHandler->Action_Add(IsActionFocused, EstimatedWaitTime, IDRegistered, m_Act_Performing->Get_Action().);
}



#pragma endregion