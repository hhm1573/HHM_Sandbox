// Fill out your copyright notice in the Description page of Project Settings.

#include "HHM_Tile_Dirt.h"

#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"



AHHM_Tile_Dirt::AHHM_Tile_Dirt(void) {
	m_ID = 2;
	m_SubID = 0;
	m_BaseTileData.IsPassable = false;

	m_BaseTileData.HP_Max = 10.0f;

	

	m_DefaultTileData.HP_Max = 10.0f;
	m_DefaultTileData.IsPassable = true;



	UMaterialInterface* pMaterial_Raw = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/HHM_Develope/Core/Resource/Temp_DyeMat"));
	UMaterialInstanceDynamic* pMaterialinstance = UMaterialInstanceDynamic::Create(pMaterial_Raw, this, TEXT("Tile_Material_Dirt"));
	pMaterialinstance->SetVectorParameterValue(TEXT("DyeColor"), FLinearColor::Red);

	m_RenderInfo.Arr_Material.Add(pMaterialinstance);
	m_RenderInfo.Num_Material = 1;
}