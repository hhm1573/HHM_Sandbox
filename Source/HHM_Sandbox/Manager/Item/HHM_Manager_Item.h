// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/HHM_Manager.h"

#include "Header/Struct_Item.h"

#include "HHM_Manager_Item.generated.h"

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API AHHM_Manager_Item : public AHHM_Manager
{
	GENERATED_BODY()

public:
	AHHM_Manager_Item();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
		TMap<int32, class UHHM_Item*> m_Container_Item;



public:
	class UHHM_Item*			Get_Item_By_ID(int32 _id);
	const class UHHM_Item*		Get_Item_By_ID_Const(int32 _id) const;

	const FHHM_ItemData&		Get_DefaultItemData_By_ID(int32 _id) const;

private:
	void Register_All_Item(void);
	
};
