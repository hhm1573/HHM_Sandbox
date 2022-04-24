// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Component_InteractionHandler.h"

#include "Manager/Interaction/HHM_Manager_Interaction.h"
#include "Entity/HHM_Entity.h"
#include "Entity/Walkable/HHM_Entity_Walkable.h"



constexpr int32 HHM_INTERACTION_HARD_LIMIT = 100;



// Sets default values for this component's properties
UHHM_Component_InteractionHandler::UHHM_Component_InteractionHandler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_Container_Action.Empty();

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

//bool UHHM_Component_InteractionHandler::Action_Start(FHHM_Data_Queue_Interaction& _queue_Interaction)
//{
//	return false;
//}

bool UHHM_Component_InteractionHandler::Action_Add(bool& _isFocused, float& _time_Wait_Estimated, int32& _id_Registered, FHHM_Data_Queue_Interaction& _action)
{
	int32 Num_Interaction = _action.m_Queue_Interaction.Num();
	if (Num_Interaction <= 0) {
		//Exception action has no interaction
		return false;
	}

	if (_action.m_Queue_Interaction[0].m_BaseData_Interaction.m_Type == EHHM_InteractionType::Type_End) {
		//Exception invalid interaction type
		return false;
	}

	

	//Prepare to add action
	int32 ID_ToAdd = Get_AvailableID();
	if (ID_ToAdd < 0) {
		//Exception can not get available action id
		return false;
	}

	int32 Priority_Add = Get_Priority_Highest();
	Priority_Add = Priority_Add + 1;
	
	//Add Action
	bool IsOccupied = m_Container_Action.Contains(ID_ToAdd);
	if (IsOccupied == true) {
		//Exception found available id was not available
		return false;
	}

	m_Container_Action.Add(ID_ToAdd, FHHM_Data_Queue_Interaction());
	m_Container_Action[ID_ToAdd].m_Priority = Priority_Add;
	m_Container_Action[ID_ToAdd].m_Queue_Interaction = _action.m_Queue_Interaction;


	
	//Prepare to add action to opponent If it has opponent
	UHHM_Component_InteractionHandler* pInteractionHandler_Opponent = m_Container_Action[ID_ToAdd].m_Queue_Interaction[0].m_Target_Interaction_Handler;
	float Time_Wait_Estimated_Opponent = 0.0f;

	if (pInteractionHandler_Opponent != nullptr) {
		FHHM_Data_Queue_Interaction Action_Reverse = _action;
		bool IsSucceed_Reverse = InteractionQueue_ReverseTarget(Action_Reverse);
		if (IsSucceed_Reverse == false) {
			//Exception can not reverse action
			m_Container_Action.Remove(ID_ToAdd);
			return false;
		}

		//Request opponent to add action
		int32 ID_RegisteredOnOpponent = -1;
		bool IsSucceed_AddToOpponent = pInteractionHandler_Opponent->Action_Add_Request(Time_Wait_Estimated_Opponent, ID_RegisteredOnOpponent, ID_ToAdd, Action_Reverse);
		if (IsSucceed_AddToOpponent == false) {
			//Exception adding action on opponent request failed
			m_Container_Action.Remove(ID_ToAdd);
			return false;
		}

		if (ID_RegisteredOnOpponent < 0) {
			//Exception action registered on invalid id at opponent
			Action_Cancel(ID_ToAdd);
			return false;
		}

		//Set ID
		m_Container_Action[ID_ToAdd].m_ID_Target = ID_RegisteredOnOpponent;
	}
	
	

	//Set return value
	_id_Registered = ID_ToAdd;
	if (m_ID_Focused == ID_ToAdd) {
		_isFocused = true;
		_time_Wait_Estimated = Time_Wait_Estimated_Opponent;
		Begin_Interaction();
	}
	else {
		float Time_Wait_Estimated = Get_EstimatedWaitTime();
		_isFocused = false;
		_time_Wait_Estimated = Time_Wait_Estimated + Time_Wait_Estimated_Opponent;
	}

	return true;
	
}

bool UHHM_Component_InteractionHandler::Action_Add_Request(float& _time_Wait_Estimated, int32& _id_Registered_Return, const int32& _id_Registered, FHHM_Data_Queue_Interaction& _action)
{
	int32 Num_Interaction = _action.m_Queue_Interaction.Num();
	if (Num_Interaction <= 0) {
		//Exception action has no interaction
		return false;
	}

	if (_action.m_Queue_Interaction[0].m_BaseData_Interaction.m_Type == EHHM_InteractionType::Type_End) {
		//Exception Invalid interaction type
		return false;
	}

	if (_id_Registered < 0) {
		//Exception interaction registered on invalid id at opponent
		return false;
	}



	int32 ID_ToAdd = Get_AvailableID();
	int32 Priority = Get_Priority_Highest() + 1;
	if (m_ID_Focused == ID_ToAdd) {
		_time_Wait_Estimated = 0.0f;
	}
	else {
		float Time_Wait_Estimated = Get_EstimatedWaitTime();
		_time_Wait_Estimated = Time_Wait_Estimated;
	}

	m_Container_Action.Add(ID_ToAdd, _action);
	m_Container_Action[ID_ToAdd].m_Priority = Priority;
	m_Container_Action[ID_ToAdd].m_ID_Target = _id_Registered;

	_id_Registered_Return = ID_ToAdd;

	return true;
}

void UHHM_Component_InteractionHandler::Action_Cancel(const int32& _id_Action)
{
	bool IsValidID = m_Container_Action.Contains(_id_Action);
	if (IsValidID == false) {
		//Exception invalid action id
		return;
	}

	int32 Num_Interaction = m_Container_Action[_id_Action].m_Queue_Interaction.Num();
	if (Num_Interaction <= 0) {
		//Exception Action contains no interactions
	}
	else {
		UHHM_Component_InteractionHandler* pInteractionHandler_Opponent = m_Container_Action[_id_Action].m_Queue_Interaction[0].m_Target_Interaction_Handler;
		if (pInteractionHandler_Opponent != nullptr) {
			int32 ID_Action_RegisteredOnOpponent = m_Container_Action[_id_Action].m_ID_Target;
			pInteractionHandler_Opponent->Action_Cancel_Request(ID_Action_RegisteredOnOpponent, this);
		}
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

	//Action valid check
	int32 Num_Interaction = m_Container_Action[_id_Action].m_Queue_Interaction.Num();
	if (Num_Interaction <= 0) {
		//Exception Action has no interaction
	}
	else {
		//Opponent valid check
		UHHM_Component_InteractionHandler* pInteractionHandler_Target = m_Container_Action[_id_Action].m_Queue_Interaction[0].m_Target_Interaction_Handler;
		if (pInteractionHandler_Target != _pRequester) {
			//Exception wrong number. cancel action requester is not matched with target action's opponent.
			return;
		}
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

bool UHHM_Component_InteractionHandler::Interact_Request(const int32& _id_Action, UHHM_Component_InteractionHandler* _pRequester)
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

	Interact_Execute(_id_Action);

	return true;
}

bool UHHM_Component_InteractionHandler::Interact_Check_Tickable(const int32& _id_Action)
{
	return false;
}

void UHHM_Component_InteractionHandler::Begin_Interaction()
{
	AActor* pOwner_Raw = nullptr;
	pOwner_Raw = GetOwner();
	if (pOwner_Raw == nullptr) {
		//Exception can not get owner
		return;
	}

	AHHM_Entity* pOwner = nullptr;
	pOwner = Cast<AHHM_Entity>(pOwner_Raw);
	if (pOwner == nullptr) {
		//Exception Owner is not entity
		return;
	}

	pOwner->Interaction_Begin();

	return;
}

void UHHM_Component_InteractionHandler::Tick_Interaction(float _deltaTime, ELevelTick _tickType)
{
	//Focused ID valid check
	bool IsValid_FocusedAction = m_Container_Action.Contains(m_ID_Focused);
	if (IsValid_FocusedAction == false) {
		//Exception Interaction handler focused action is NULL
		int32 Num_Action = m_Container_Action.Num();
		if (Num_Action > 0) {
			FocusNextAction();
		}
		else {
			End_Interaction();
		}
		return;
	}

	int32 Num_Interaction_FocusedAction = m_Container_Action[m_ID_Focused].m_Queue_Interaction.Num();
	if (Num_Interaction_FocusedAction <= 0) {
		//Exception focused action has no interaction
		Action_Cancel(m_ID_Focused);
		return;
	}

	

	//Tick the interaction
	TArray<int32> Array_ID_Compatible = Get_List_CompatibleID(m_ID_Focused);		//상호작용이 launch되면 삭제되기 때문에 미리 호환상호작용 리스트를 받아옴.

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
		//Check whether focused interact can be ticked or wait
		bool IsTickable = true;
		if (m_Container_Action[m_ID_Focused].m_Queue_Interaction[0].m_Target_Interaction_Handler != nullptr) {
			int32 ID_Registered_Focuse = m_Container_Action[m_ID_Focused].m_ID_Target;
			IsTickable = m_Container_Action[m_ID_Focused].m_Queue_Interaction[0].m_Target_Interaction_Handler->Interact_Check_Tickable(ID_Registered_Focuse);
		}



		if (IsTickable) {
			float Timer_Focused_After = m_Container_Action[m_ID_Focused].m_Queue_Interaction[0].m_Timer_Interaction;
			Timer_Focused_After = Timer_Focused_After - _deltaTime;
			if (Timer_Focused_After < 0.0f) {
				bool IsSucceed_LaunchInteraction = Interact_Launch(m_ID_Focused);										//Launch interact
				if (IsSucceed_LaunchInteraction == false) {
					//Exception Launching interact failed
					return;
				}
			}
			else {
				m_Container_Action[m_ID_Focused].m_Queue_Interaction[0].m_Timer_Interaction = Timer_Focused_After;			//Reduce interaction timer
			}
		}
	}
	


	//Tick compatible interactions
	for (int32 id_Compatible : Array_ID_Compatible) {
		bool IsValidID_Compatible = m_Container_Action.Contains(id_Compatible);
		if (IsValidID_Compatible == false) {
			//Exception invalid compatible action id
			continue;
		}

		int32 Num_Interaction_Compatible = m_Container_Action[id_Compatible].m_Queue_Interaction.Num();
		if (Num_Interaction_Compatible <= 0) {
			//Exception compatible action has no interactions
			Action_Cancel(id_Compatible);
			continue;
		}

		

		bool IsWaitingOpponent_Compatible = m_Container_Action[id_Compatible].m_Queue_Interaction[0].m_bShouldWait;
		if (IsWaitingOpponent_Compatible == true) {
			continue;
		}

		UHHM_Component_InteractionHandler* pInteractionHandler_Opponnent_Compatible = m_Container_Action[id_Compatible].m_Queue_Interaction[0].m_Target_Interaction_Handler;
		if (pInteractionHandler_Opponnent_Compatible != nullptr) {
			int32 ID_Registered_Compatible = m_Container_Action[id_Compatible].m_ID_Target;
			bool IsTickable_Compatible = m_Container_Action[id_Compatible].m_Queue_Interaction[0].m_Target_Interaction_Handler->Interact_Check_Tickable(ID_Registered_Compatible);
			if (IsTickable_Compatible == false) {
				continue;
			}
		}

		float Timer_After_Compatible = m_Container_Action[id_Compatible].m_Queue_Interaction[0].m_Timer_Interaction;
		Timer_After_Compatible = Timer_After_Compatible - _deltaTime;
		if (Timer_After_Compatible < 0.0f) {
			bool IsSucceed_Launch_Compatible = Interact_Launch(id_Compatible);
			if (IsSucceed_Launch_Compatible == false) {
				//Exception launching compatible interaction failed
				continue;
			}
		}
		else {
			m_Container_Action[id_Compatible].m_Queue_Interaction[0].m_Timer_Interaction = Timer_After_Compatible;
		}

	}

	return;
}

void UHHM_Component_InteractionHandler::End_Interaction()
{
	int32 Num_Action = m_Container_Action.Num();
	if (Num_Action > 0) {
		//Exception End_Interaction called while there is remaining action
		return;
	}



	m_ID_Focused = 0;
	m_Container_Action.Empty();



	AActor* pOwner_Raw = nullptr;
	pOwner_Raw = GetOwner();
	if (pOwner_Raw == nullptr) {
		//Exception can not get owner
		return;
	}

	AHHM_Entity* pOwner = nullptr;
	pOwner = Cast<AHHM_Entity>(pOwner_Raw);
	if (pOwner == nullptr) {
		//Exception Owner is not entity
		return;
	}

	pOwner->Interaction_End();

	return;
}

bool UHHM_Component_InteractionHandler::Interact_Launch(const int32& _id_Action)
{
	//Valid Check
	bool IsValidID = m_Container_Action.Contains(_id_Action);
	if (IsValidID == false) {									// Note : 한 Interaction Handler 에서도 두번 이상씩 Action id 유효검사를 실행하는 상황이긴 하다. (Launch -> Execute, Request -> Execute)
		//Exception Invalid action id
		return false;
	}

	//Interaction queue valid check
	int32 Num_Interaction = m_Container_Action[_id_Action].m_Queue_Interaction.Num();
	if (Num_Interaction <= 0) {
		//Exception Action has no interactions
		return false;
	}

	

	//Request interact if possible
	UHHM_Component_InteractionHandler* pInteractionHandler_Opponent = nullptr;
	pInteractionHandler_Opponent = m_Container_Action[_id_Action].m_Queue_Interaction[0].m_Target_Interaction_Handler;
	if (pInteractionHandler_Opponent != nullptr) {
		int32 ID_Action_RegisteredOnOpponent = -1;
		ID_Action_RegisteredOnOpponent = m_Container_Action[_id_Action].m_ID_Target;
		bool IsSucceed_Request = pInteractionHandler_Opponent->Interact_Request(ID_Action_RegisteredOnOpponent, this);
		if (IsSucceed_Request == false) {
			//Exception Interaction request failed. Action Canceled (Action Data : ~~~~)
			Action_Cancel(_id_Action);
			return false;
		}
	}



	//Execute interaction
	Interact_Execute(_id_Action);

	return true;
}

void UHHM_Component_InteractionHandler::Interact_Execute(const int32& _id_Action)
{
	//Action id valid check
	bool IsValidID = m_Container_Action.Contains(_id_Action);
	if (IsValidID == false) {
		//Exception Invalid action id
		return;
	}

	//Interaction valid check
	int32 Num_Interaction = m_Container_Action[_id_Action].m_Queue_Interaction.Num();
	if (Num_Interaction <= 0) {
		//Exception action has no interaction
		Action_Cancel(_id_Action);
		return;
	}



	//Execute Interact
	FHHM_Data_Interaction& Data_Interaction_Executing = m_Container_Action[_id_Action].m_Queue_Interaction[0];
	AHHM_Manager_Interaction::Execute_Interact(Data_Interaction_Executing, this);

	

	//Pop front interaction
	m_Container_Action[_id_Action].m_Queue_Interaction.RemoveAt(0);

	//is action finished?
	int32 Num_Interaction_Remain = m_Container_Action[_id_Action].m_Queue_Interaction.Num();
	if (Num_Interaction_Remain <= 0) {
		//Send signal to owner before remove action
		Action_End(_id_Action);



		//Remove finished action
		m_Container_Action.Remove(_id_Action);

		//is there any remaining action?
		int32 Num_Action = m_Container_Action.Num();
		if (Num_Action > 0) {
			//change focus to next action
			FocusNextAction();
		}
		else {
			//No Remaining action. fininsh the interaction
			End_Interaction();
		}
	}

	return;

}



void UHHM_Component_InteractionHandler::Action_End(const int32& _id_Action)
{
	AActor* pOwner_Raw = nullptr;
	pOwner_Raw = GetOwner();
	if (pOwner_Raw == nullptr) {
		//Exception No Owner
		return;
	}

	AHHM_Entity_Walkable* pOwner = nullptr;
	pOwner = Cast<AHHM_Entity_Walkable>(pOwner_Raw);
	if (pOwner == nullptr) {
		//Exception cant cast owner to hhm entity
		return;
	}

	pOwner->Action_End(m_Container_Action[_id_Action]);

	return;
}

void UHHM_Component_InteractionHandler::Action_Canceled(const int32& _id_Action)
{
	AActor* pOwner_Raw = nullptr;
	pOwner_Raw = GetOwner();
	if (pOwner_Raw == nullptr) {
		//Exception No Owner
		return;
	}

	AHHM_Entity_Walkable* pOwner = nullptr;
	pOwner = Cast<AHHM_Entity_Walkable>(pOwner_Raw);
	if (pOwner == nullptr) {
		//Exception cant cast owner to hhm entity
		return;
	}

	pOwner->Action_Canceled(m_Container_Action[_id_Action]);

	return;
}



bool UHHM_Component_InteractionHandler::InteractionQueue_ReverseTarget(FHHM_Data_Queue_Interaction& _queue_Interaction)
{
	int32 Num_Interaction = _queue_Interaction.m_Queue_Interaction.Num();
	if (Num_Interaction <= 0) {
		//Exception action has no interaction
		return false;
	}

	for (int32 index_Reverse = 0; index_Reverse < Num_Interaction; ++index_Reverse) {
		if (_queue_Interaction.m_Queue_Interaction[index_Reverse].m_Target_Interaction_Handler != nullptr) {
			_queue_Interaction.m_Queue_Interaction[index_Reverse].m_bShouldWait = !_queue_Interaction.m_Queue_Interaction[index_Reverse].m_bShouldWait;
		}
	}

	return true;
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

void UHHM_Component_InteractionHandler::Abort_Action(int32 _id_Action)
{
	bool IsValidID = m_Container_Action.Contains(_id_Action);
	if (IsValidID == false) {
		//Exception invalid action id
		return;
	}

	Action_Canceled(_id_Action);

	m_Container_Action.Remove(_id_Action);
}

void UHHM_Component_InteractionHandler::Request_AbortAction(int32 _id_Action)
{
	bool IsValidID = m_Container_Action.Contains(_id_Action);
	if (IsValidID == false) {
		//Exception invalid action id
		return;
	}

	int32 Num_Interaction = m_Container_Action[_id_Action].m_Queue_Interaction.Num();
	if (Num_Interaction <= 0) {
		//Exception Action has no interaction
	}
	else {
		UHHM_Component_InteractionHandler* pInteractionHandler_Target = m_Container_Action[_id_Action].m_Queue_Interaction[0].m_Target_Interaction_Handler;
		if (pInteractionHandler_Target != nullptr) {
			int32 ID_Action_RegisteredOnOpponnet = m_Container_Action[_id_Action].m_ID_Target;
			pInteractionHandler_Target->Action_Cancel_Request(ID_Action_RegisteredOnOpponnet, this);
		}
	}

	return;
}

TArray<int32> UHHM_Component_InteractionHandler::Get_List_CompatibleID(int32 _id_Action)
{
	return TArray<int32>();
}

int32 UHHM_Component_InteractionHandler::Get_AvailableID()
{
	for (int32 index_Check = 0; index_Check < HHM_INTERACTION_HARD_LIMIT; ++index_Check) {
		bool IsOccupied = m_Container_Action.Contains(index_Check);
		if (IsOccupied == false) {
			return index_Check;
		}
	}

	return -1;
}

int32 UHHM_Component_InteractionHandler::Get_Priority_Highest()
{
	int32 Priority_Highest = -1;
	for (auto& pair : m_Container_Action) {
		int32 Priority = pair.Value.m_Priority;
		if (Priority > Priority_Highest) {
			Priority_Highest = Priority;
		}
	}

	return Priority_Highest;
}

float UHHM_Component_InteractionHandler::Get_EstimatedWaitTime()
{
	float EstimatedWaitTime = 0.0f;

	for (auto& pair : m_Container_Action) {
		int32 Num_Interaction = pair.Value.m_Queue_Interaction.Num();
		for (int32 index_Interaction = 0; index_Interaction < Num_Interaction; ++index_Interaction) {
			float Timer_Interaction = pair.Value.m_Queue_Interaction[index_Interaction].m_Timer_Interaction;
			EstimatedWaitTime = EstimatedWaitTime + Timer_Interaction;
		}
	}

	return EstimatedWaitTime;
}

