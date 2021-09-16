// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Data/Item/ItemData/HHM_ItemData.h"
#include "Header/Enum_Item.h"
#include "Data/Item/RenderData/HHM_RenderData_Item.h"

#include "HHM_Item.generated.h"

/**
 * 아이템의 기능만을 수행하는 클래스.
 * 개별 아이템의 정보는 ItemData 클래스에서 데이터만을 따로 관리한다.
 */
UCLASS()
class HHM_SANDBOX_API UHHM_Item : public UObject
{
	GENERATED_BODY()

public:
	UHHM_Item();

protected:
	//UI 에 아이템 렌더용 텍스쳐 변수 필요. Textrue2D UTexture 등 여러 텍스쳐 타입중 어느것이 필요할줄 몰라 추후 UI작업시에 확정예정.
	UPROPERTY()
		int32						m_ItemID = -1;		//Desired Item ID. Should be set by item self.
	UPROPERTY()
		int32						m_ItemSubID = 0;
	UPROPERTY()
		int32						m_ItemSize_Horizontal = 1;		//On Inventory
	UPROPERTY()
		int32						m_ItemSize_Vertical = 1;

	UPROPERTY()
		UHHM_ItemData*				m_DefaultItemData = nullptr;	//필요없을수도?
	UPROPERTY()
		EHHM_ItemType				m_ItemType = EHHM_ItemType::Item_Unavailiable;

	//Rendering Data
	UPROPERTY()
		FHHM_RenderData_Item		m_RenderData;
	// HHM Note : 상황에 따라서 인벤토리에 있는 아이템의 텍스쳐를 변환시킬 필요가 있다.(아이템을 접었다 핀다거나 아이템의 갯수에따라 텍스쳐가 바뀌는 등) 
	// 인벤토리 UI작업을 하며 해당 부분에 대해 생각해보자

	UPROPERTY()
		TArray<FString>				m_Container_Descriptions;



public:
	int32					Get_ItemID() const { return m_ItemID; }
	int32					Get_ItemSubID() const { return m_ItemSubID; }
	FIntPoint				Get_ItemSize() const { return FIntPoint(m_ItemSize_Horizontal, m_ItemSize_Vertical); }
	const UHHM_ItemData*	Get_DefaultItemData() const { return m_DefaultItemData; }

public:
	virtual bool			Create_NewItemData(TSharedPtr<UHHM_ItemData>& _pReturn);		//Return DefaultItemData

public:
	/*
	Called on when someone uses item. usually called from entity that using this item.
	no need to set valid value on both _targetLocaiton and _pTarget. because depends on item, item will use only one of them.
	return true will reduce item use count and might consume item.
	*/
	virtual bool	On_ItemUsed(class ALocalMap* _pLocalMap, class AEntity* _pUser, UHHM_ItemData* _itemUsed, FVector _targetLocation, class AEntity* _pTarget); // HHM Note : Make it as pure virtual



	//Return RenderData that ItemActor can use.
	virtual void	Get_RenderData_ItemActor(class UMaterialInterface*& _material_Return, FVector2D& _cubeSize_Return, class ALocalMap* _pLocalMap, const UHHM_ItemData* _itemData);
	
};
