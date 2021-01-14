// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/HHM_Manager.h"

#include "Data/Item/ItemData/HHM_ItemData.h"

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

	// HHM Note : 아이템을 생성할때 아이템으로부터 아이템 데이터를 요구하게끔 해놓았기에 이 함수가 사실상 쓸모가 없을 수 있다. 추후 상황을 봐서 삭제를 결정.
	const UHHM_ItemData*		Get_DefaultItemData_By_ID(int32 _id) const;		

private:
	void Register_All_Item(void);
	
};
