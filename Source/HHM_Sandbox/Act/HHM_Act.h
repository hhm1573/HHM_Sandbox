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
	//act가 취소되었을 때 호출. 아이템을 옮기는 액션 도중 같은 상황 이었을 경우 Root Inventory(손) 의 아이템을 바닥에 버리거나 가방에 넣는식으로 뒷정리하는 액션을 반환함.
	virtual void	On_Cancel();

	FHHM_Data_Action&	Get_Action() { return m_CurrentAction; }
	bool				Get_IsCanceled() { return m_Canceled; }

public:
	void	Set_Target(class AHHM_Entity* _pInteractor, class AHHM_Entity* _pTarget_Entity = nullptr, FVector _targetLocation = FVector::ZeroVector);
	
};
