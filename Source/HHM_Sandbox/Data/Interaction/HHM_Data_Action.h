#pragma once

#include "CoreMinimal.h"

#include "HHM_Data_Queue_Interaction.h"

#include "HHM_Data_Action.generated.h"



USTRUCT()
struct FHHM_Data_Action
{
	GENERATED_USTRUCT_BODY();



public:
	UPROPERTY()
		FHHM_Data_Queue_Interaction	m_Queue_Interaction;

	UPROPERTY()
		FVector					m_Location_Target = FVector::ZeroVector;
	UPROPERTY()
		float					m_AcceptableRadius = 0.0f;



public:
	FHHM_Data_Action() {
		m_Queue_Interaction.m_Queue_Interaction.Empty();
	}
};