// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Data/LocalMap/LocalMap.h"
#include "Header/Struct_Pathfinder.h"

#include "HHM_PathFinder.generated.h"

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API UHHM_PathFinder : public UObject
{
	GENERATED_BODY()
	
public:
	static TArray<FHHM_PathNodeData> Search_Path(const ALocalMap* _pLocalMap, FVector2D _location_Start, FVector2D _location_End, int32 _entitySize_Width, int32 _entitySize_Height, int32 _maxJumpLength, int32 _maxFallLength, int32 _maxHorizontalJumpLength = 1);

};
