// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Header/Struct_Item.h"
#include "Header/Struct_RenderInfo.h"

#include "HHM_Item.generated.h"

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API UHHM_Item : public UObject
{
	GENERATED_BODY()

public:
	UHHM_Item();

protected:
	//UI 에 아이템 렌더용 텍스쳐 변수 필요. Textrue2D UTexture 등 여러 텍스쳐 타입중 어느것이 필요할줄 몰라 추후 UI작업시에 확정예정.
	FHHM_ItemData				m_DefaultItemData;

	//Rendering Data
	FHHM_RenderInfo_ItemActor	m_RenderInfo;	
	// HHM Note : 상황에 따라서 인벤토리에 있는 아이템의 텍스쳐를 변환시킬 필요가 있다.(아이템을 접었다 핀다거나 아이템의 갯수에따라 텍스쳐가 바뀌는 등) 
	// 인벤토리 UI작업을 하며 해당 부분에 대해 생각해보자

	TArray<FString>				m_Container_Descriptions;

	bool						m_bUnlimited_Usage = 0;
	float						m_Distance_ItemUsable = 0.0f;

	FVector2D					m_Size_Actor = FVector2D::ZeroVector;



public:
	const FHHM_ItemData&	Get_DefaultItemData() { return m_DefaultItemData; }
	float					Get_ItemUsableDistance() { return m_Distance_ItemUsable; }

public:
	/*
	Called on when someone uses item. usually called from entity that using this item.
	no need to set valid value on both _targetLocaiton and _pTarget. because depends on item, item will use only one of them.
	return true will reduce item use count and might consume item.
	*/
	virtual bool	On_ItemUsed(class ALocalMap* _pLocalMap, class AEntity* _pUser, FVector _targetLocation, class AEntity* _pTarget); // HHM Note : Make it as pure virtual
	
};
