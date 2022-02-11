// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Tile_Debug_Ladder.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine/Texture.h"
#include "Materials/MaterialInstanceDynamic.h"



AHHM_Tile_Debug_Ladder::AHHM_Tile_Debug_Ladder() {
	m_ID = 10000;
	m_SubID = 0;



	m_DefaultTileData.HP_Max = 5.0f;
	
	m_DefaultTileData.IsPassable = true;
	m_DefaultTileData.IsLadder = true;
	m_DefaultTileData.IsScaffold = true;
	m_SpeedMultiplier_Ladder = 1.0f;



	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Asset_Material(TEXT("/Game/HHM_Develope/Core/Resource/Mat_Tile_Seamless"));
	if (Asset_Material.Succeeded() == true) {
		static ConstructorHelpers::FObjectFinder<UTexture> Asset_Texture(TEXT("/Game/HHM_Develope/Core/Resource/Texture/Develope/Ladder"));
		if (Asset_Texture.Succeeded() == true) {

			UMaterialInstanceDynamic* pMaterialInstance = UMaterialInstanceDynamic::Create(Asset_Material.Object, this, TEXT("Tile_Material_Ladder"));
			pMaterialInstance->SetTextureParameterValue(TEXT("Param_Texture"), Asset_Texture.Object);
			pMaterialInstance->SetScalarParameterValue(TEXT("Param_TextureSize"), 64.0f);
			
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



void AHHM_Tile_Debug_Ladder::OnConstruction(const FTransform& Transform)
{
}
