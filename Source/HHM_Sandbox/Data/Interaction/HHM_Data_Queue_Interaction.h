#pragma once

#include "CoreMinimal.h"

#include "HHM_Data_Interaction.h"

#include "HHM_Data_Queue_Interaction.generated.h"



USTRUCT(Blueprintable)
struct FHHM_Data_Queue_Interaction
{
	GENERATED_USTRUCT_BODY();



public:
	UPROPERTY()
		TArray<FHHM_Data_Interaction>	m_Queue_Interaction;
	UPROPERTY(BlueprintReadOnly)
		// ��� InteractionHandler �� ��ϵ�, �� Action Data�� �����ϴ� ID.
		// �� Action�� ��� Action queue�� ��ϵ� ID�� ��Ÿ��.
		int32							m_ID_Target = -1;
	UPROPERTY()
		int32							m_Priority = -1;
	UPROPERTY()
		AActor*							m_pInteractor = nullptr;

	UPROPERTY(BlueprintReadOnly)
		//Name of Action. (same name as HHM_Data_Action has)
		FString							m_Name;



public:
	FHHM_Data_Queue_Interaction() {
		m_Queue_Interaction.Empty();
	}
};