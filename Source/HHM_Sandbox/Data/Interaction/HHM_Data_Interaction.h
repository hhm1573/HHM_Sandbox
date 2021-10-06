#pragma once

#include "CoreMinimal.h"

//#include "Base/Component/InteractionHandler/HHM_Component_InteractionHandler.h"
#include "HHM_Data_Interaction_Base.h"

#include "HHM_Data_Interaction.generated.h"



/*
* 
*/
USTRUCT()
struct FHHM_Data_Interaction
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY()
		FHHM_Data_Interaction_Base	m_BaseData_Interaction;

	/*UPROPERTY()
		FVector		m_Location_Target = FVector::ZeroVector;
	UPROPERTY()
		float		m_AcceptableRadius = 0.0f;*/

	UPROPERTY()
		float		m_Timer_Interaction = 0.0f;

	UPROPERTY()
		bool		m_bShouldWait = false;
	UPROPERTY()
		//Cancel the interaction if this timer ends. set it larget than interaction timer so that opponent don't cancel interaction.
		//this timer is for prevent to wating corrupted interaction by some reason.
		float		m_Timer_EndWait = 0.0f;

	UPROPERTY()
		// HHM Note : ������ �ϳ��� Action�� ���� Ÿ�ٰ� ��ȣ�ۿ� ������ ������ �����Ƿ� ���� Ÿ�ٺκ��� HHM_Data_Queue_Interaction (AKA Action) ���� �Űܼ� �޸𸮸� �����̶� �����ص� �ɵ�.
		// �ϴ��� �۵���Ű�� ���� ������� �˵��� �ö� �����Ǹ� ��Ȳ���� ����.
		FVector		m_Target_Interaction_Location = FVector::ZeroVector;
	UPROPERTY()
		AActor*		m_Target_Interaction_Actor = nullptr;
	UPROPERTY()
		class UHHM_Component_InteractionHandler* m_Target_Interaction_Handler = nullptr;



public:
	FHHM_Data_Interaction() {

	}
};