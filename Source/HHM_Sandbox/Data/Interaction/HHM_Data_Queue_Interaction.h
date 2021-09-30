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
		// 상대 InteractionHandler 에 등록된, 이 Action Data와 대응하는 ID.
		// 이 Action이 상대 Action queue에 등록된 ID를 나타냄.
		int32							m_ID_Target = -1;
	UPROPERTY()
		int32							m_Priority = -1;



public:
	FHHM_Data_Queue_Interaction() {
		m_Queue_Interaction.Empty();
	}
};