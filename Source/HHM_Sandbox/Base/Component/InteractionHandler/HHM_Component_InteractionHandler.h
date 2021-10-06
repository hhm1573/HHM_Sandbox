// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Data/Interaction/HHM_Data_Queue_Interaction.h"

#include "HHM_Component_InteractionHandler.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HHM_SANDBOX_API UHHM_Component_InteractionHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHHM_Component_InteractionHandler();



private:
	UPROPERTY()
		TMap<int32, FHHM_Data_Queue_Interaction>		m_Container_Action;
	UPROPERTY()
		int32											m_ID_Focused = 0;
	//UPROPERTY()
		//int32											m_ID_Next = 0;		//if new interaction added before handler finish all the interaction, newly added interaction will have this id.



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



public:
	/*
	* Called from entity.
	* Entity가 Act를 갖고 Act에서 받은 Action위치에 이동후 InteractionHandler 에게 Action큐를 넘겨 Action을 진행.
	*/
	//bool	Action_Start(FHHM_Data_Queue_Interaction& _queue_Interaction);

	/*
	* @Return : return true if queue added.
	*/
	bool	Action_Add(bool& _isFocused, float& _time_Wait_Estimated, int32& _id_Registered, FHHM_Data_Queue_Interaction& _action);	//첫 액션이면 Begin interaction 호출

	bool	Action_Add_Request(float& _time_Wait_Estimated, int32& _id_Registered_Return, const int32& _id_Registered, FHHM_Data_Queue_Interaction& _action);

	void	Action_Cancel(const int32& _id_Action);		//마지막 액션이면 End interaction 호출

	void	Action_Cancel_Request(const int32& _id_Action, UHHM_Component_InteractionHandler* _pRequester);



public:
	//Opponent interaction handler 에서 호출되는 interact 함수. 유효검사를 진행한 후 유효할경우 자신의 Interact_Execute를 호출한다.
	//유효하지 않을경우 false를 반환한다.
	bool	Interact_Request(const int32& _id_Action, UHHM_Component_InteractionHandler* _pRequester);

	bool	Interact_Check_Tickable(const int32& _id_Action);




protected:
	void	Begin_Interaction();

	void	Tick_Interaction(float _deltaTime, ELevelTick _tickType);
	
	void	End_Interaction();	//ID focused, next 리셋

	//Tick 등에서 자기자신이 호출하기 위한 Interact 함수. 상대 Interaction Handler가 있을경우 상대의 Interact_Request 함수를 호출한다.
	//상대 Interaction Handler가 없거나 Interact_Request에서 true를 반환받을 경우 Interact_Execute를 실행한다.
	//false를 반환받을 경우 현재 Action을 취소하고 false를 반환하여 Tick함수에서 compatible interaction을 진행하지 않게 한다.
	bool	Interact_Launch(const int32& _id_Action);
	
	//자신의 Interact 작업을 진행하는 함수. Interaction manager의 Interact인터페이스를 통해 Interact 작업을 수행하고, 다음 Interaction 으로 넘어가거나 Action이 종료되었을 경우 다음 Action으로 넘어간다.
	void	Interact_Execute(const int32& _id_Action);



protected:
	void	Action_End(const int32& _id_Action);

	void	Action_Canceled(const int32& _id_Action);


protected:
	bool	InteractionQueue_ReverseTarget(FHHM_Data_Queue_Interaction& _queue_Interaction);

	/*
	* Reduce priority of all the actions, find action with lowest priority then set it as member variation which "m_ID_Focused"
	*/
	void	FocusNextAction();

	//void	AbortFocusedAction();

	void	Abort_Action(int32 _id_Action);

	//Request to opponent to cancel action
	void	Request_AbortAction(int32 _id_Action);

	TArray<int32>	Get_List_CompatibleID(int32 _id_Action);



	int32	Get_AvailableID();

	int32	Get_Priority_Highest();

	float	Get_EstimatedWaitTime();


		
};
