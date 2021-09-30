// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Data/Interaction/HHM_Data_Queue_Interaction.h"

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



public:
	virtual void	Begin_Act(FHHM_Data_Queue_Interaction& _queue_Interaction_Return);
	virtual void	Performed_Action(const bool& _isActEnd_Return, FHHM_Data_Queue_Interaction& _queue_Interaction_Return);
	
};
