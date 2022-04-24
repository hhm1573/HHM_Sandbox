// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Manager/Interaction/HHM_Manager_Interaction.h"
#include "Data/Interaction/HHM_Data_Action.h"

#include "HHM_Act.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class HHM_SANDBOX_API UHHM_Act : public UObject				// HHM To Do : Tick �Լ� ���� Actor�� ���� ���÷� ��ġ�� ���ϴ� Ÿ���� �����Ҽ� �ְԲ� ����.
{
	GENERATED_BODY()

public:
	UHHM_Act();



protected:
	UPROPERTY()
		class AHHM_Entity*	m_pInteractor = nullptr;

	UPROPERTY()
		class AHHM_Entity*	m_pTarget_Entity = nullptr;
	UPROPERTY()
		//Index Location of target tile
		FVector				m_Target_Location = FVector::ZeroVector;

	UPROPERTY()
		//AI���� ���� �׼��� �Ϸ�Ǿ����� ���� �Ǵ��ϱ� ���� ����. ������ Action�� ���� ��쿡�� ��� ������. Performed_Action �Լ��� ȣ��� ������ 1�� ����.
		int32				m_CurrentActionIndex = 0;
	UPROPERTY()
		FHHM_Data_Action	m_CurrentAction = FHHM_Data_Action();

	UPROPERTY()
		bool				m_Canceled = false;

public:
	UFUNCTION(BlueprintCallable)
		void	BP_Set_Target(class AHHM_Entity* _pInteractor, class AHHM_Entity* _pTarget_Entity = nullptr, FVector _targetLocation = FVector::ZeroVector);



public:
	virtual bool	Begin_Act();
	//Return true if action is finished. �߰� �׼��� ���� Act�� �Ϸ� �Ǿ��� ��� true�� ��ȯ.
	virtual bool	Performed_Action();

	//Called when act got canceled. Expect to return clean-up action. for example ) put item to inventory or throw away from root inventory(Hand), when item transferring action canceled.
	//act�� ��ҵǾ��� �� ȣ��. �������� �ű�� �׼� ���� ���� ��Ȳ �̾��� ��� Root Inventory(��) �� �������� �ٴڿ� �����ų� ���濡 �ִ½����� �������ϴ� �׼��� ��ȯ��.
	virtual void	On_Cancel();

	UFUNCTION(BlueprintCallable)
		FHHM_Data_Action&	Get_Action() { return m_CurrentAction; }
	UFUNCTION(BlueprintCallable)
		int32				Get_CurrentActionIndex() { return m_CurrentActionIndex; }
	bool				Get_IsCanceled() { return m_Canceled; }

public:
	void	Set_Target(class AHHM_Entity* _pInteractor, class AHHM_Entity* _pTarget_Entity = nullptr, FVector _targetLocation = FVector::ZeroVector);



protected:
	class AHHM_Manager_Interaction* Get_Manager_Interaction();
	
};
