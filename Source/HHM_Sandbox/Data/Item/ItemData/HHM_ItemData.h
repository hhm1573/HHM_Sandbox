// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HHM_ItemData.generated.h"

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API UHHM_ItemData : public UObject
{
	GENERATED_BODY()
	//GENERATED_UCLASS_BODY()

public:
	UHHM_ItemData();



protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
		int32				m_ID = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
		int32				m_SubID = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
		class UHHM_Item*	m_Item = nullptr;

public:
	class UHHM_Item* Get_Item() { return m_Item; }

public:
	void Set_ID(int32 _id) { m_ID = _id; }
	void Set_SubID(int32 _subID) { m_SubID = _subID; }
	void Set_Item(class UHHM_Item* _pItem) { m_Item = _pItem; }

	
};
