// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Data/Interaction/HHM_Data_Action.h"

#include "HHM_Act.generated.h"

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API UHHM_Act : public UObject
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
		FHHM_Data_Action	m_CurrentAction = FHHM_Data_Action();

	UPROPERTY()
		bool				m_Canceled = false;



public:
	virtual bool	Begin_Act();
	virtual void	Performed_Action(const bool& _isActEnd_Return);

	//Called when act got canceled. Expect to return clean-up action. for example ) put item to inventory or throw away from root inventory(Hand), when item transferring action canceled.
	//act�� ��ҵǾ��� �� ȣ��. �������� �ű�� �׼� ���� ���� ��Ȳ �̾��� ��� Root Inventory(��) �� �������� �ٴڿ� �����ų� ���濡 �ִ½����� �������ϴ� �׼��� ��ȯ��.
	virtual void	On_Cancel();

	FHHM_Data_Action&	Get_Action() { return m_CurrentAction; }
	bool				Get_IsCanceled() { return m_Canceled; }

public:
	void	Set_Target(class AHHM_Entity* _pInteractor, class AHHM_Entity* _pTarget_Entity = nullptr, FVector _targetLocation = FVector::ZeroVector);
	
};
