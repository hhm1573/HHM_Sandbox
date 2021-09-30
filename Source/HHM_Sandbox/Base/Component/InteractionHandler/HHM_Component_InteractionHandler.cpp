// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Component_InteractionHandler.h"

// Sets default values for this component's properties
UHHM_Component_InteractionHandler::UHHM_Component_InteractionHandler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_Queue_Action.Empty();

	// ...
}


// Called when the game starts
void UHHM_Component_InteractionHandler::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHHM_Component_InteractionHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	

	//Do tick if has any remaining action
	int32 Num_Actions = m_Container_Action.Num();
	if (Num_Actions > 0) {
		Tick_Interaction(DeltaTime, TickType);
	}


}

bool UHHM_Component_InteractionHandler::Action_Start(FHHM_Data_Queue_Interaction& _queue_Interaction)
{
	return false;
}

bool UHHM_Component_InteractionHandler::Action_Add(bool& _isFocused, float& _time_Wait_Estimated, int32& _id_Registered, FHHM_Data_Queue_Interaction& _queue_Interaction)
{
	return false;
}

void UHHM_Component_InteractionHandler::Action_Cancel(const int32& _id_Action)
{
	bool IsValidID = m_Container_Action.Contains(_id_Action);
	if (IsValidID == false) {
		//Exception invalid action id
		return;
	}

	UHHM_Component_InteractionHandler* pInteractionHandler_Opponent = m_Container_Action[_id_Action].m_Queue_Interaction[0].m_Target_Interaction_Handler;
	if (pInteractionHandler_Opponent != nullptr) {
		int32 ID_Action_RegisteredOnOpponent = m_Container_Action[_id_Action].m_ID_Target;
		pInteractionHandler_Opponent->Action_Cancel_Request(ID_Action_RegisteredOnOpponent, this);
	}



	//Abort Action
	Abort_Action(_id_Action);

	//Check if canceled action was focused one
	if (_id_Action == m_ID_Focused) {
		//end interaction if it was last action. focus next interaction other else
		int32 Num_RemainingAction = m_Container_Action.Num();
		if (Num_RemainingAction <= 0) {
			End_Interaction();
		}
		else {
			FocusNextAction();
		}
	}

	return;
}

void UHHM_Component_InteractionHandler::Action_Cancel_Request(const int32& _id_Action, UHHM_Component_InteractionHandler* _pRequester)
{
	//ID Valid Check
	bool IsValidID = m_Container_Action.Contains(_id_Action);
	if (IsValidID == false) {
		//Exception Invalid action id. may caused by corrupted action data
		return;
	}

	//Opponent valid check
	UHHM_Component_InteractionHandler* pInteractionHandler_Target = m_Container_Action[_id_Action].m_Queue_Interaction[0].m_Target_Interaction_Handler;
	if (pInteractionHandler_Target != _pRequester) {
		//Exception wrong number. cancel action requester is not matched with target action's opponent.
		return;
	}



	//Abort Action
	Abort_Action(_id_Action);

	//check if canceled action is focused action
	if (m_ID_Focused == _id_Action) {
		//if there is no remaining action, end interaction
		int32 Num_Action = m_Container_Action.Num();
		if (Num_Action <= 0) {
			End_Interaction();
		}
		else {
			//Move on to next action
			FocusNextAction();
		}
	}

	return;
}

bool UHHM_Component_InteractionHandler::Execute_Interact(const int32& _id_Action, UHHM_Component_InteractionHandler* _pRequester)
{
	//Valid check
	bool IsValidID = m_Container_Action.Contains(_id_Action);
	if (IsValidID == false) {
		//Exception invalid action id
		return false;
	}

	//Requester check
	UHHM_Component_InteractionHandler* pInteractionHandler_Opponent = m_Container_Action[_id_Action].m_Queue_Interaction[0].m_Target_Interaction_Handler;
	if (_pRequester != pInteractionHandler_Opponent) {
		//Exception Requester does not match
		return false;
	}

	Interact(_id_Action);

	return true;
}

void UHHM_Component_InteractionHandler::Begin_Interaction()
{
}

void UHHM_Component_InteractionHandler::Tick_Interaction(float _deltaTime, ELevelTick _tickType)
{
	//Focused ID valid check
	bool IsValid_FocusedAction = m_Container_Action.Contains(m_ID_Focused);
	if (IsValid_FocusedAction == false) {
		//Exception Interaction handler focused action is NULL
		return;
	}

	

	//Tick the interaction
	bool IsWaitingOpponent = m_Container_Action[m_ID_Focused].m_Queue_Interaction[0].m_bShouldWait;
	if (IsWaitingOpponent) {
		//Wait opponent's to interact
		float Timer_Wait_Focused_After = m_Container_Action[m_ID_Focused].m_Queue_Interaction[0].m_Timer_EndWait;
		Timer_Wait_Focused_After = Timer_Wait_Focused_After - _deltaTime;
		if (Timer_Wait_Focused_After < 0.0f) {
			//Exception Interaction wait limit timer expired. it could mean interaction data is corrupted.
			Action_Cancel(m_ID_Focused);	//Action_Cancel에 다음 액션으로 포커스를 이동하거나 end_Interaction을 호출하는 부분이 포함되어있음.
			return;							//m_ID_Focsued가 변경되었거나 End_Interaction이 호출되었으므로 호환 상호작용을 그냥 스킵하고 넘어감. 호환 상호작용들에 대해 현재 Tick만큼 딜레이가 생길순 있음.
		}
	}
	else {
		float Timer_Focused_After = m_Container_Action[m_ID_Focused].m_Queue_Interaction[0].m_Timer_Interaction;
		Timer_Focused_After = Timer_Focused_After - _deltaTime;
		if (Timer_Focused_After < 0.0f) {
			Interact(m_ID_Focused);
		}
		m_Container_Action[m_ID_Focused].m_Queue_Interaction[0].m_Timer_Interaction = Timer_Focused_After;
	}
	
}

void UHHM_Component_InteractionHandler::End_Interaction()
{
}

void UHHM_Component_InteractionHandler::Interact(const int32& _id_Action)
{
	//상호작용 수행후 다음 큐로 넘어가거나 다음 액션으로 넘어가는 작업.
}

bool UHHM_Component_InteractionHandler::InteractionQueue_ReverseTarget(FHHM_Data_Queue_Interaction& _queue_Interaction)
{
	return false;
}

void UHHM_Component_InteractionHandler::FocusNextAction()
{
	//Check if action queue is empty
	//int32 Num_Actions = m_Container_Action.Num();
	//if (Num_Actions < 1) {
	//	//Exception No remaining actions
	//	return;
	//}
	TArray<int32> Array_Keys;
	Array_Keys.Empty();
	int32 Num_Keys = m_Container_Action.GetKeys(Array_Keys);
	if (Num_Keys < 1) {
		//Exception No Remaining actions
		return;
	}



	//Lower priorities
	for (auto& pair : m_Container_Action) {
		pair.Value.m_Priority = pair.Value.m_Priority - 1;
	}



	//Get Lowest priority
	int32 ID_LowestPriority = Array_Keys[0];
	int32 Priority_Lowest = m_Container_Action[ID_LowestPriority].m_Priority;
	for (auto& pair : m_Container_Action) {
		int32 Priority_Check = pair.Value.m_Priority;
		if (Priority_Check < Priority_Lowest) {
			ID_LowestPriority = pair.Key;
			Priority_Lowest = Priority_Check;
		}
	}

	m_ID_Focused = ID_LowestPriority;

	return;
}

void UHHM_Component_InteractionHandler::AbortFocusedAction()
{
}

void UHHM_Component_InteractionHandler::Abort_Action(int32 _id_Action)
{
	bool IsValidID = m_Container_Action.Contains(_id_Action);
	if (IsValidID == false) {
		//Exception invalid action id
		return;
	}

	m_Container_Action.Remove(_id_Action);
}

void UHHM_Component_InteractionHandler::Request_AbortAction(int32 _id_Action)
{
	bool IsValidID = m_Container_Action.Contains(_id_Action);
	if (IsValidID == false) {
		//Exception invalid action id
		return;
	}

	UHHM_Component_InteractionHandler* pInteractionHandler_Target = m_Container_Action[_id_Action].m_Queue_Interaction[0].m_Target_Interaction_Handler;
	if (pInteractionHandler_Target != nullptr) {
		int32 ID_Action_RegisteredOnOpponnet = m_Container_Action[_id_Action].m_ID_Target;
		pInteractionHandler_Target->Action_Cancel_Request(ID_Action_RegisteredOnOpponnet, this);
	}
}

TArray<int32> UHHM_Component_InteractionHandler::Get_List_CompatibleID(int32 _id_Action)
{
	return TArray<int32>();
}

