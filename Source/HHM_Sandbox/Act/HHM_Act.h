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



public:
	virtual bool	Begin_Act(FHHM_Data_Action& _data_Action_Return);
	virtual void	Performed_Action(const bool& _isActEnd_Return, FHHM_Data_Action& _data_Action_Return);

public:
	void	Set_Target(class AHHM_Entity* _pInteractor, class AHHM_Entity* _pTarget_Entity = nullptr, FVector _targetLocation = FVector::ZeroVector);
	
};
