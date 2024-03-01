#pragma once

#include "CoreMinimal.h"
#include "HHM_Data_Inventory.generated.h"

USTRUCT(BlueprintType)
struct FHHM_Data_Inventory
{
	GENERATED_USTRUCT_BODY()

public:
	FHHM_Data_Inventory() {

	}

public:
	UPROPERTY(BlueprintReadWrite)
		FString		m_Name;
	UPROPERTY(BlueprintReadWrite)
		float		m_Time_Interact = 0.0f;

	UPROPERTY(BlueprintReadWrite)
		int32		m_Size_Horizontal = 0;
	UPROPERTY(BlueprintReadWrite)
		int32		m_Size_Vertical = 0;



public:
	void Initialize(const FString& _name, const float& _time_Interact, const int32& _size_Horizontal, const int32& _size_Vertical) {
		m_Name = _name;
		m_Time_Interact = _time_Interact;
		m_Size_Horizontal = _size_Horizontal;
		m_Size_Vertical = _size_Vertical;
	}

};