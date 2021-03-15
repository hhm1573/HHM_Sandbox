// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Data/LocalMap/LocalMap.h"
#include "Header/Struct_Pathfinder.h"
#include "PathFinder/HHM_Parameter_PathFind.h"

#include "HHM_PathFinder.generated.h"

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API UHHM_PathFinder : public UObject
{
	GENERATED_BODY()
	
public:
	static TArray<FHHM_PathNodeData> Search_Path(const ALocalMap* _pLocalMap, FVector2D _location_Start, FVector2D _location_End, const FHHM_Parameter_PathFind& _pathfindParams);

private:

};
