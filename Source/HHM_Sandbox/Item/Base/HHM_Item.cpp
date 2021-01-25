// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Item.h"

#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "UObject/ConstructorHelpers.h"



UHHM_Item::UHHM_Item(void) {
	m_DefaultItemData = CreateDefaultSubobject<UHHM_ItemData>(TEXT("DefaultItemData"));

	

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/HHM_Develope/Core/Resource/Temp_DyeMat"));
	if (MaterialAsset.Succeeded() == true) {
		UMaterialInstanceDynamic* pMaterialInstance = UMaterialInstanceDynamic::Create(MaterialAsset.Object, this, TEXT("Material_Item_Debug"));
		FLinearColor Color_Purple = FLinearColor(1.0f, 0.0f, 1.0f);
		pMaterialInstance->SetVectorParameterValue(TEXT("DyeColor"), Color_Purple);

		m_RenderData.Container_Material.Add(pMaterialInstance);
		
	}

	m_RenderData.Size_ItemActor = FVector2D(5.0f, 5.0f);
}

bool UHHM_Item::Create_NewItemData(TSharedPtr<UHHM_ItemData>& _pReturn)
{
	if (m_ItemID < 0) {
		return false;
	}

	if (_pReturn == nullptr) {
		_pReturn = TSharedPtr<UHHM_ItemData>(NewObject<UHHM_ItemData>());
		if (_pReturn == nullptr) {
			//Exception NewObject Failed
			return false;
		}
	}

	_pReturn->Set_ID(m_ItemID);
	_pReturn->Set_SubID(m_ItemSubID);
	_pReturn->Set_Item(this);

	return true;
}

bool UHHM_Item::On_ItemUsed(ALocalMap* _pLocalMap, AEntity* _pUser, UHHM_ItemData* _itemUsed, FVector _targetLocation, AEntity* _pTarget)
{
	return false;
}

void UHHM_Item::Get_RenderData_ItemActor(UMaterialInterface*& _material_Return, FVector2D& _cubeSize_Return, ALocalMap* _pLocalMap, const UHHM_ItemData* _itemData)
{
	int32 Num_Material = m_RenderData.Container_Material.Num();
	if (Num_Material > 0) {
		_material_Return = m_RenderData.Container_Material[0];
	}

	_cubeSize_Return = m_RenderData.Size_ItemActor;
}
