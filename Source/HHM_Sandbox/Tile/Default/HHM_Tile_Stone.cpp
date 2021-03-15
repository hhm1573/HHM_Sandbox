// Fill out your copyright notice in the Description page of Project Settings.

#include "HHM_Tile_Stone.h"

#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Texture.h"



AHHM_Tile_Stone::AHHM_Tile_Stone(void) {
	m_ID = 1;
	m_SubID = 0;
}

void AHHM_Tile_Stone::BeginPlay(void) {
	Super::BeginPlay();
}

void AHHM_Tile_Stone::OnConstruction(const FTransform& Transform) {
	m_DefaultTileData.HP_Max = 20.0f;
	m_DefaultTileData.HP = m_DefaultTileData.HP_Max;

	m_DefaultTileData.IsPassable = false;



	UMaterialInterface* pMaterial_Raw = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/HHM_Develope/Core/Resource/Mat_Tile_Seamless"));
	if (IsValid(pMaterial_Raw) == true) {
		UTexture*			pTexture_Stone = LoadObject<UTexture>(nullptr, TEXT("/Game/HHM_Develope/Core/Resource/Texture/Texture_Tile_Stone_Develope"));
		UMaterialInstanceDynamic* pMaterialinstance = UMaterialInstanceDynamic::Create(pMaterial_Raw, this, TEXT("Tile_Material_Stone"));
		pMaterialinstance->SetTextureParameterValue(TEXT("Param_Texture"), pTexture_Stone);
		pMaterialinstance->SetScalarParameterValue(TEXT("Param_TextureSize"), 64.0f);
		/*FLinearColor Color_Grey = FLinearColor(1.0f, 0.5f, 0.5f, 1.0f);
		pMaterialinstance->SetVectorParameterValue(TEXT("DyeColor"), Color_Grey);*/


		m_RenderInfo.eRenderType = EHHM_RenderType::RType_Instanced;
		m_RenderInfo.Arr_Material.Add(pMaterialinstance);
		m_RenderInfo.Num_Material = 1;
	}
}