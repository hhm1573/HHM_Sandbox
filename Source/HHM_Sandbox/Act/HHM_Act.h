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
class HHM_SANDBOX_API UHHM_Act : public UObject				// HHM To Do : Tick 함수 만들어서 Actor와 같이 수시로 위치가 변하는 타겟을 추적할수 있게끔 구현.
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
		//AI에서 현재 액션이 완료되었는지 등을 판단하기 위한 변수. 복수의 Action을 갖는 경우에도 사용 가능함. Performed_Action 함수가 호출될 때마다 1씩 증가.
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
	//Return true if action is finished. 추가 액션이 없어 Act가 완료 되었을 경우 true를 반환.
	virtual bool	Performed_Action();

	//Called when act got canceled. Expect to return clean-up action. for example ) put item to inventory or throw away from root inventory(Hand), when item transferring action canceled.
	//act가 취소되었을 때 호출. 아이템을 옮기는 액션 도중 같은 상황 이었을 경우 Root Inventory(손) 의 아이템을 바닥에 버리거나 가방에 넣는식으로 뒷정리하는 액션을 반환함.
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
