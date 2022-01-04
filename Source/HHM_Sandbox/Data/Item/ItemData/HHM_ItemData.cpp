// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_ItemData.h"

#include "Manager/Item/HHM_Manager_Item.h"



UHHM_ItemData::UHHM_ItemData() {
	m_ItemName = AHHM_Manager_Item::ItemName_Base;



	/*static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/HHM_Develope/Core/Resource/Temp_DyeMat"));
	if (MaterialAsset.Succeeded() == true) {
		UMaterialInstanceDynamic* pMaterialInstance = UMaterialInstanceDynamic::Create(MaterialAsset.Object, this, TEXT("Material_Item_Debug"));
		FLinearColor Color_Purple = FLinearColor(1.0f, 0.0f, 1.0f);
		pMaterialInstance->SetVectorParameterValue(TEXT("DyeColor"), Color_Purple);

		m_RenderData.Container_Material.Add(pMaterialInstance);

	}*/
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Asset_Material(TEXT("/Game/HHM_Develope/Core/Resource/Mat_Tile_Seamless"));
	if (Asset_Material.Succeeded() == true) {
		static ConstructorHelpers::FObjectFinder<UTexture> Asset_Texture(TEXT("/Game/HHM_Develope/Core/Resource/Texture/Develope/Ladder"));
		if (Asset_Texture.Succeeded() == true) {

			UMaterialInstanceDynamic* pMaterialInstance = UMaterialInstanceDynamic::Create(Asset_Material.Object, this, TEXT("Tile_Material_Ladder"));
			pMaterialInstance->SetTextureParameterValue(TEXT("Param_Texture"), Asset_Texture.Object);
			pMaterialInstance->SetScalarParameterValue(TEXT("Param_TextureSize"), 64.0f);

			m_RenderData.Container_Material.Add(pMaterialInstance);

		}
		else {
			//Exception Cant find texture
		}
	}
	else {
		//Exception Cant find material
	}

	m_RenderData.Size_ItemActor = FVector2D(5.0f, 5.0f);

	m_ItemSize_Horizontal = 1;
	m_ItemSize_Vertical = 1;
}

void UHHM_ItemData::Initialize_RenderData()
{
}



void UHHM_ItemData::BP_Get_RenderData(UMaterialInterface*& _material_Return, ALocalMap* _pLocalMap)
{
	if (_pLocalMap == nullptr) {
		//Exception
		return;
	}

	FVector2D TmpVec2D = FVector2D::ZeroVector;
	Get_RenderData_ItemActor(_material_Return, TmpVec2D, _pLocalMap);
}

void UHHM_ItemData::Get_RenderData_ItemActor(UMaterialInterface*& _material_Return, FVector2D& _cubeSize_Return, ALocalMap* _pLocalMap)
{
	int32 Num_Material = m_RenderData.Container_Material.Num();
	if (Num_Material > 0) {
		_material_Return = m_RenderData.Container_Material[0];
	}

	_cubeSize_Return = m_RenderData.Size_ItemActor;
}
