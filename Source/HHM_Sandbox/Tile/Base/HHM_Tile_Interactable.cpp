// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Tile_Interactable.h"

#include "Engine/Engine.h"



AHHM_Tile_Interactable::AHHM_Tile_Interactable() {

}



void AHHM_Tile_Interactable::On_Interact(ALocalMap* _pLocalMap, FHHM_TileData& _tileData, AEntity* _pEntity, FHHM_Data_Interact* _pinteractData)
{
	FString StrPrint = TEXT("");
	StrPrint.Printf(TEXT("Tile Interacted ! Tile Location (%d : %d)"), _tileData.Index_Horizontal, _tileData.Index_Vertical);
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, StrPrint);
	}
}
