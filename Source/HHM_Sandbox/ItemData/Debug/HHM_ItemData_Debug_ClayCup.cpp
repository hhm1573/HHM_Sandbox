// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_ItemData_Debug_ClayCup.h"

#include "Manager/Item/HHM_Manager_Item.h"



UHHM_ItemData_Debug_ClayCup::UHHM_ItemData_Debug_ClayCup() {
	m_ID = 0;
	m_SubID = 0;

	m_ItemName = AHHM_Manager_Item::ItemName_Base;



	/*static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/HHM_Develope/Core/Resource/Temp_DyeMat"));
	if (MaterialAsset.Succeeded() == true) {
		UMaterialInstanceDynamic* pMaterialInstance = UMaterialInstanceDynamic::Create(MaterialAsset.Object, this, TEXT("Material_Item_Debug"));
		FLinearColor Color_Purple = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
		pMaterialInstance->SetVectorParameterValue(TEXT("DyeColor"), FLinearColor::Blue);

		m_RenderData.Container_Material.Add(pMaterialInstance);

	}*/

	m_RenderData.Size_ItemActor = FVector2D(5.0f, 5.0f);

	m_ItemSize_Horizontal = 1;
	m_ItemSize_Vertical = 1;
}