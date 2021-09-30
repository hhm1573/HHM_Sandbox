#pragma once

#include "CoreMinimal.h"

#include "HHM_Data_Interaction.h"

#include "HHM_Data_Queue_Interaction.generated.h"



USTRUCT()
struct FHHM_Data_Queue_Interaction
{
	GENERATED_USTRUCT_BODY();



public:
	UPROPERTY()
		TArray<FHHM_Data_Interaction>	m_Queue_Interaction;
	UPROPERTY()
		// ��� InteractionHandler �� ��ϵ�, �� Action Data�� �����ϴ� ID.
		// �� Action�� ��� Action queue�� ��ϵ� ID�� ��Ÿ��.
		int32							m_ID_Target = -1;
	UPROPERTY()
		int32							m_Priority = -1;



public:
	FHHM_Data_Queue_Interaction() {
		m_Queue_Interaction.Empty();
	}
};