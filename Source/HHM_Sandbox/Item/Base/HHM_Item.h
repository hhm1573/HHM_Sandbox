// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Data/Item/ItemData/HHM_ItemData.h"
#include "Header/Enum_Item.h"
#include "Data/Item/RenderData/HHM_RenderData_Item.h"

#include "HHM_Item.generated.h"

/**
 * �������� ��ɸ��� �����ϴ� Ŭ����.
 * ���� �������� ������ ItemData Ŭ�������� �����͸��� ���� �����Ѵ�.
 */
UCLASS()
class HHM_SANDBOX_API UHHM_Item : public UObject
{
	GENERATED_BODY()

public:
	UHHM_Item();

protected:
	//UI �� ������ ������ �ؽ��� ���� �ʿ�. Textrue2D UTexture �� ���� �ؽ��� Ÿ���� ������� �ʿ����� ���� ���� UI�۾��ÿ� Ȯ������.
	UPROPERTY()	//Item Name which will be registered on item manager
		FString						m_ItemName;		
	UPROPERTY()
		EHHM_ItemType				m_ItemType = EHHM_ItemType::Item_Unavailiable;

	//Rendering Data
	UPROPERTY()
		FHHM_RenderData_Item		m_RenderData;
	// HHM Note : ��Ȳ�� ���� �κ��丮�� �ִ� �������� �ؽ��ĸ� ��ȯ��ų �ʿ䰡 �ִ�.(�������� ������ �ɴٰų� �������� ���������� �ؽ��İ� �ٲ�� ��) 
	// �κ��丮 UI�۾��� �ϸ� �ش� �κп� ���� �����غ���

	UPROPERTY()
		TArray<FString>				m_Container_Descriptions;



public:
	FString					Get_ItemName() const { return m_ItemName; }

public:
	/*
	Called on when someone uses item. usually called from entity that using this item.
	no need to set valid value on both _targetLocaiton and _pTarget. because depends on item, item will use only one of them.
	return true will reduce item use count and might consume item.
	*/
	virtual bool	On_ItemUsed(class ALocalMap* _pLocalMap, class AEntity* _pUser, UHHM_ItemData* _itemUsed, FVector _targetLocation, class AEntity* _pTarget) const; // HHM Note : Make it as pure virtual



	//Return RenderData that ItemActor can use.
	virtual void	Get_RenderData_ItemActor(class UMaterialInterface*& _material_Return, FVector2D& _cubeSize_Return, class ALocalMap* _pLocalMap, const UHHM_ItemData* _itemData) const;
	
};
