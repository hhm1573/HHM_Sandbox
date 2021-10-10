// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile/Base/HHM_Tile.h"

#include "Data/Interaction/HHM_Data_Interact.h"

#include "HHM_Tile_Interactable.generated.h"

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API AHHM_Tile_Interactable : public AHHM_Tile
{
	GENERATED_BODY()
	


public:
	AHHM_Tile_Interactable();



public:
	virtual void On_Interact(class ALocalMap* _pLocalMap, FHHM_TileData& _tileData, class AEntity* _pEntity, FHHM_Data_Interact* _pinteractData);
};
