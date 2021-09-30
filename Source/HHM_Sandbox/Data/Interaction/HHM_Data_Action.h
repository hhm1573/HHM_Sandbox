#pragma once

#include "CoreMinimal.h"

#include "HHM_Data_Interaction.h"

#include "HHM_Data_Action.generated.h"



USTRUCT()
struct FHHM_Data_Action
{
	GENERATED_USTRUCT_BODY();



public:
	UPROPERTY()
		FHHM_Data_Interaction	m_Data_Interaction;

	UPROPERTY()
		FVector					m_Location_Target = FVector::ZeroVector;
	UPROPERTY()
		float					m_AcceptableRadius = 0.0f;



public:
	FHHM_Data_Action() {

	}
};