#pragma once

#include "CoreMinimal.h"

#include "HHM_Const_Interaction.h"
#include "HHM_Enum_InteractionType.h"

#include "HHM_Data_Interaction_Base.generated.h"



USTRUCT()
struct FHHM_Data_Interaction_Base		//Contains data for registering to manager
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY()
		int32					m_ID = HHM_INTERACTION_ID_INVALID;
	UPROPERTY()
		FString					m_Name;
	UPROPERTY()
		EHHM_InteractionType	m_Type = EHHM_InteractionType::Type_End;

	UPROPERTY()
		float					m_Time = 0.0f;
	
	UPROPERTY()
		TArray<int32>					m_Container_ID_Compatible;
	UPROPERTY()
		TArray<EHHM_InteractionType>	m_Container_Type_Compatible;
	


public:
	FHHM_Data_Interaction_Base() {
		m_Container_ID_Compatible.Empty();
		m_Container_Type_Compatible.Empty();
		m_Name.Empty();
	}

public:
	void Set_BaseData(const int32& _id_Interaction, const FString& _name, const EHHM_InteractionType& _eType_Interaction, const float& _time_Interaction) {
		m_ID = _id_Interaction;
		m_Name = _name;
		m_Type = _eType_Interaction;
		m_Time = _time_Interaction;
	}

	void Add_Compatible_ID(int32 _id_Interaction) {
		m_Container_ID_Compatible.Add(_id_Interaction);
	}

	void Add_Compatible_Type(const EHHM_InteractionType& _eType_Interaction) {
		m_Container_Type_Compatible.Add(_eType_Interaction);
	}

};