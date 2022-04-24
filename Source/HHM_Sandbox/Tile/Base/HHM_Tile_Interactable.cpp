// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Tile_Interactable.h"

#include "Engine/Engine.h"



AHHM_Tile_Interactable::AHHM_Tile_Interactable() {
	m_ID = 9998;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Asset_Material(TEXT("/Game/HHM_Develope/Core/Resource/Mat_Tile_Seamless"));
	if (Asset_Material.Succeeded() == true) {
		static ConstructorHelpers::FObjectFinder<UTexture> Asset_Texture(TEXT("/Game/HHM_Develope/Core/Resource/Texture/Develope/Tile/Tile_Debug_Interact"));
		if (Asset_Texture.Succeeded() == true) {

			UMaterialInstanceDynamic* pMaterialInstance = UMaterialInstanceDynamic::Create(Asset_Material.Object, this, TEXT("Tile_Material_Debug_Interact"));
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

	m_DefaultTileData.IsPassable = true;
}



void AHHM_Tile_Interactable::On_Interact(ALocalMap* _pLocalMap, FHHM_TileData& _tileData, AHHM_Entity* _pEntity, FHHM_Data_Interact* _pinteractData)
{
	FString StrPrint = TEXT("");
	StrPrint.Printf(TEXT("Tile Interacted ! Tile Location (%d : %d)"), _tileData.Index_Horizontal, _tileData.Index_Vertical);
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, StrPrint);
	}
}
