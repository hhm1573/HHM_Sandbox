#pragma once

#include "CoreMinimal.h"

#include "HHM_Data_Queue_Interaction.h"

#include "HHM_Data_Action.generated.h"



USTRUCT(Blueprintable)
struct FHHM_Data_Action
{
	GENERATED_USTRUCT_BODY();



public:
	UPROPERTY(BlueprintReadOnly)
		FHHM_Data_Queue_Interaction	m_Queue_Interaction;

	UPROPERTY(BlueprintReadOnly)
		FVector					m_Location_Target = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly)
		float					m_AcceptableRadius = 0.0f;

	UPROPERTY()	//Interaction Handler Component에 등록된 ID.
		int32					m_InteractionID = -1;



public:
	FHHM_Data_Action() {
		m_Queue_Interaction.m_Queue_Interaction.Empty();
	}
};