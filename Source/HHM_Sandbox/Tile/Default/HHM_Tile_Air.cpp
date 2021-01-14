// Fill out your copyright notice in the Description page of Project Settings.

#include "HHM_Tile_Air.h"

#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"



AHHM_Tile_Air::AHHM_Tile_Air(void) {
	m_ID = 0;
	m_BaseTileData.IsPassable = true;



	UMaterialInterface* pMaterial_Raw = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/HHM_Develope/Core/Resource/Mat_Air"));

	m_RenderInfo.eRenderType = EHHM_RenderType::RType_Instanced;
	m_RenderInfo.Arr_Material.Add(pMaterial_Raw);
	m_RenderInfo.Num_Material = 1;
	m_RenderInfo.IsNeedToRender = false;
}