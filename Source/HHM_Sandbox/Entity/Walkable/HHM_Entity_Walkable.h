// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/HHM_Entity.h"

#include "Act/HHM_Act.h"

#include "HHM_Entity_Walkable.generated.h"



class UHHM_Component_Movement;



/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API AHHM_Entity_Walkable : public AHHM_Entity
{
	GENERATED_BODY()

public:
	AHHM_Entity_Walkable();

private:
	UPROPERTY(Category = "HHM_Entity", VisibleAnywhere)
		UHHM_Component_Movement* m_pComponent_Movement = nullptr;

protected:
	//UPROPERTY()	//Act�� �����Ͽ� Interact ť�� ������������ ����.
	//	bool				m_IsInAction = false;
	UPROPERTY()
		UHHM_Act* m_Act_Performing = nullptr;
	/*UPROPERTY()
		int32				m_ID_PerformingAction = -1;*/
	UPROPERTY()
		TArray<UHHM_Act*>	m_Queue_Act;



public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;



public:
	//Add Move Function
	UFUNCTION(BlueprintCallable)
		bool	Order_MoveToLocation(int32 _index_Horizontal, int32 _index_Vertical);

public:
	UFUNCTION(BlueprintCallable)
		float	Get_MoveTimer();	// HHM Note : Fix this functions name something like "Get_MoveComponent_MoveTimer", "MoveComponent_Get_MoveTimer" or "Component_Movement_Get_MoveTimer" etc.
	UFUNCTION(BlueprintCallable)
		UHHM_Component_Movement* Get_Component_Movement();



#pragma region Act

public:
	UFUNCTION(BlueprintCallable)
		UHHM_Act* Get_PerformingAct() { return m_Act_Performing; }
	UFUNCTION(BlueprintCallable)
		//return true if Action Started
		bool		BP_Action_Perform(UHHM_Act*& _act_Return, int32& _actionIndex_Return);
	UFUNCTION(BlueprintCallable)
		//Return true if Action finished
		bool		BP_Action_Tick(const UHHM_Act* _act_Last, int32 _actionIndex_Last);

	UFUNCTION(BlueprintCallable)
		void		BP_Debug_Act_Queue_Add(UHHM_Act* _pAct);

public:
	//Called when performing action finished.
	virtual void Action_End(FHHM_Data_Queue_Interaction& _action);
	//Canceled�� ��ǻ� �ʿ� �������� ������. ������ ���� ���� Interaction Begin End ���� �̷������ Act�� Action�� �����ڸ��� �����ֱ⿡ �ǽ����ڰ� Canceled �Լ����� ������ ���� ����
	//������ ���� Act�� ��ҵɶ� ���� �������� Action�� ������ Act�� ��ó�� �������� ����ǰ� �̴� Tick�Լ����� �˾Ƽ� ����ǹǷ� Action ������ �̿��� ���� �ʿ� ���µ�
	//Action�� ������ �����ϰ� ��ȣ�ۿ��� �����ϴ� ���� Interaction Handler Component �̴�. �ٵ� Act�� ��ҵɶ� �̹� ������Ʈ�� ���� Action�� ���� abort �۾��� �����ϱ� ������ ������ ���� ������ ����.
	virtual void Action_Canceled(FHHM_Data_Queue_Interaction& _action);



public://Act Interface
	//Force to start act. cancel queued acts
	void Act_Start(UHHM_Act* _pAct);
	//Cancel current action
	void Act_Cancel();
	void Act_Queue_Add(UHHM_Act* _pAct);
	void Act_Queue_Clear();

	const UHHM_Act* Get_Act_Performing() const { return m_Act_Performing; }

protected:
	void Act_Tick(float DeltaTime);
	void Act_Begin(UHHM_Act* _pAct);

#pragma endregion
};
