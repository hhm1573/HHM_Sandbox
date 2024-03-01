// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/HHM_Manager.h"

#include "Data/Item/HHM_Container_SubItem.h"
#include "Data/Item/ItemData/HHM_ItemData.h"
#include "Data/Item/HHM_Container_ItemData.h"

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

public:
	static const FString ItemName_Base;
	static const FString ItemName_Throwable;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
		TMap<FString, UHHM_Item*>				m_Container_Item;
	UPROPERTY()
		TMap<int32, FHHM_Container_ItemData>	m_Container_ItemData;



public:
	UFUNCTION(BlueprintCallable)
		class UHHM_ItemData*		BP_Create_Default_ItemData_By_ID(int32 _id, int32 _subID = 0);



public:
	const class UHHM_Item*			Get_Item_By_Name(FString _name) const;
	const class UHHM_ItemData*		Get_ItemData_By_ID(int32 _id, int32 _subID = 0) const;

	class UHHM_ItemData*			Create_Default_ItemData_By_ID(int32 _id, int32 _subID = 0);

	// HHM Note : �������� �����Ҷ� ���������κ��� ������ �����͸� �䱸�ϰԲ� �س��ұ⿡ �� �Լ��� ��ǻ� ���� ���� �� �ִ�. ���� ��Ȳ�� ���� ������ ����. [deprecated]
	//const UHHM_ItemData*		Get_DefaultItemData_By_ID(int32 _id, int32 _subID = 0) const;		

private:
	void Register_All_Item(void);
	void Register_All_ItemData(void);
	
};