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
	UPROPERTY()
		int32											m_ID_Next = 0;		//if new interaction added before handler finish all the interaction, newly added interaction will have this id.



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
	bool	Action_Start(FHHM_Data_Queue_Interaction& _queue_Interaction);

	/*
	* @Return : return true if queue added.
	*/
	bool	Action_Add(bool& _isFocused, float& _time_Wait_Estimated, int32& _id_Registered, FHHM_Data_Queue_Interaction& _queue_Interaction);	//첫 액션이면 Begin interaction 호출

	void	Action_Cancel(const int32& _id_Action);		//마지막 액션이면 End interaction 호출

	void	Action_Cancel_Request(const int32& _id_Action, UHHM_Component_InteractionHandler* _pRequester);



public:
	//외부에서 호출하기위한 Interact 함수. 자기자신의 interact만 실행한다.
	bool	Execute_Interact(const int32& _id_Action, UHHM_Component_InteractionHandler* _pRequester);



protected:
	void	Begin_Interaction();

	void	Tick_Interaction(float _deltaTime, ELevelTick _tickType);
	
	void	End_Interaction();	//ID focused, next 리셋

	//Tick 등에서 자기자신이 호출하기 위한 Interact 함수. 상대 Interaction Handler가 있을경우 상대의 Execute_Interact 함수를 호출한다.
	//Execute_Interact 함수에서 false를 반환받을 경우 Action을 취소한다.
	//수정 : 자기 자신의 interact 수행. Execute_Interact 는 Tick 등에서 상대
	void	Interact(const int32& _id_Action);


protected:
	bool	InteractionQueue_ReverseTarget(FHHM_Data_Queue_Interaction& _queue_Interaction);

	/*
	* Reduce priority of all the actions, find action with lowest priority then set it as member variation which "m_ID_Focused"
	*/
	void	FocusNextAction();

	void	AbortFocusedAction();

	void	Abort_Action(int32 _id_Action);

	//Request to opponent to cancel action
	void	Request_AbortAction(int32 _id_Action);

	TArray<int32>	Get_List_CompatibleID(int32 _id_Action);


		
};
