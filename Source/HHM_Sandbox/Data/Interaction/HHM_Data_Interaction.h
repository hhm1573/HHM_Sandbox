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
		// HHM Note : 어차피 하나의 Action이 여러 타겟과 상호작용 할일이 없을것 같으므로 추후 타겟부분을 HHM_Data_Queue_Interaction (AKA Action) 으로 옮겨서 메모리를 조금이라도 절약해도 될듯.
		// 일단은 작동시키고 추후 어느정도 궤도에 올라 안정되면 상황보고 수정.
		FVector		m_Target_Interaction_Location = FVector::ZeroVector;
	UPROPERTY()
		AActor*		m_Target_Interaction_Actor = nullptr;
	UPROPERTY()
		class UHHM_Component_InteractionHandler* m_Target_Interaction_Handler = nullptr;



public:
	FHHM_Data_Interaction() {

	}
};