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
	* Entity�� Act�� ���� Act���� ���� Action��ġ�� �̵��� InteractionHandler ���� Actionť�� �Ѱ� Action�� ����.
	*/
	bool	Action_Start(FHHM_Data_Queue_Interaction& _queue_Interaction);

	/*
	* @Return : return true if queue added.
	*/
	bool	Action_Add(bool& _isFocused, float& _time_Wait_Estimated, int32& _id_Registered, FHHM_Data_Queue_Interaction& _queue_Interaction);	//ù �׼��̸� Begin interaction ȣ��

	void	Action_Cancel(const int32& _id_Action);		//������ �׼��̸� End interaction ȣ��

	void	Action_Cancel_Request(const int32& _id_Action, UHHM_Component_InteractionHandler* _pRequester);



public:
	//�ܺο��� ȣ���ϱ����� Interact �Լ�. �ڱ��ڽ��� interact�� �����Ѵ�.
	bool	Execute_Interact(const int32& _id_Action, UHHM_Component_InteractionHandler* _pRequester);



protected:
	void	Begin_Interaction();

	void	Tick_Interaction(float _deltaTime, ELevelTick _tickType);
	
	void	End_Interaction();	//ID focused, next ����

	//Tick ��� �ڱ��ڽ��� ȣ���ϱ� ���� Interact �Լ�. ��� Interaction Handler�� ������� ����� Execute_Interact �Լ��� ȣ���Ѵ�.
	//Execute_Interact �Լ����� false�� ��ȯ���� ��� Action�� ����Ѵ�.
	//���� : �ڱ� �ڽ��� interact ����. Execute_Interact �� Tick ��� ���
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
