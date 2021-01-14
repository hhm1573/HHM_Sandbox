// Fill out your copyright notice in the Description page of Project Settings.

#include "HHM_Tile_Dirt.h"

#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"



AHHM_Tile_Dirt::AHHM_Tile_Dirt(void) {
	m_ID = 2;
	m_BaseTileData.IsPassable = false;

	m_BaseTileData.HP_Max = 10.0f;



	UMaterialInterface* pMaterial_Raw = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/HHM_Develope/Core/Resource/Temp_DyeMat"));
	UMaterialInstanceDynamic* pMaterialinstance = UMaterialInstanceDynamic::Create(pMaterial_Raw, this, TEXT("Tile_Material_Dirt"));
	pMaterialinstance->SetVectorParameterValue(TEXT("DyeColor"), FLinearColor::Red);

	m_RenderInfo.eRenderType = EHHM_RenderType::RType_Instanced;
	m_RenderInfo.Arr_Material.Add(pMaterialinstance);
	m_RenderInfo.Num_Material = 1;
}