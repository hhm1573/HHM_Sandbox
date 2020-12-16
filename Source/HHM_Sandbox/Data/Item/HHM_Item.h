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
	//UI �� ������ ������ �ؽ��� ���� �ʿ�. Textrue2D UTexture �� ���� �ؽ��� Ÿ���� ������� �ʿ����� ���� ���� UI�۾��ÿ� Ȯ������.
	FHHM_ItemData				m_DefaultItemData;

	//Rendering Data
	FHHM_RenderInfo_ItemActor	m_RenderInfo;	
	// HHM Note : ��Ȳ�� ���� �κ��丮�� �ִ� �������� �ؽ��ĸ� ��ȯ��ų �ʿ䰡 �ִ�.(�������� ������ �ɴٰų� �������� ���������� �ؽ��İ� �ٲ�� ��) 
	// �κ��丮 UI�۾��� �ϸ� �ش� �κп� ���� �����غ���

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
