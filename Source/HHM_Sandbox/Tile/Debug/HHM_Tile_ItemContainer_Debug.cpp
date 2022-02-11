// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Tile_ItemContainer_Debug.h"



AHHM_Tile_ItemContainer_Debug::AHHM_Tile_ItemContainer_Debug() {
	m_ID = 9999;

	m_Data_Inventory.m_Size_Horizontal = 5;
	m_Data_Inventory.m_Size_Vertical = 3;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Asset_Material(TEXT("/Game/HHM_Develope/Core/Resource/Mat_Tile_Seamless"));
	if (Asset_Material.Succeeded() == true) {
		static ConstructorHelpers::FObjectFinder<UTexture> Asset_Texture(TEXT("/Game/HHM_Develope/Core/Resource/Texture/Develope/Tile/Tile_Container_Debug"));
		if (Asset_Texture.Succeeded() == true) {

			UMaterialInstanceDynamic* pMaterialInstance = UMaterialInstanceDynamic::Create(Asset_Material.Object, this, TEXT("Tile_Material_Debug_Container"));
			pMaterialInstance->SetTextureParameterValue(TEXT("Param_Texture"), Asset_Texture.Object);
			pMaterialInstance->SetScalarParameterValue(TEXT("Param_TextureSize"), 10.0f);

			m_RenderInfo.Arr_Material.Add(pMaterialInstance);
			m_RenderInfo.Num_Material = 1;
		}
		else {
			//Exception Cant find texture
		}
	}
	else {
		//Exception Cant find material
	}
}