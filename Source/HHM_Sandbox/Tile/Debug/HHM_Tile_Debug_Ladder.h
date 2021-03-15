// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile/Base/HHM_Tile.h"
#include "HHM_Tile_Debug_Ladder.generated.h"

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API AHHM_Tile_Debug_Ladder : public AHHM_Tile
{
	GENERATED_BODY()

public:
	AHHM_Tile_Debug_Ladder();

public:
	virtual void OnConstruction(const FTransform& Transform) override;
	
};
