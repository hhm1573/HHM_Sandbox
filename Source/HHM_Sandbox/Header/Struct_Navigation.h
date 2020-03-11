#pragma once

#include "CoreMinimal.h"
#include "Struct_Navigation.generated.h"

USTRUCT()
struct FHHM_NavLinkInfo
{
	GENERATED_USTRUCT_BODY()

public:
	TArray<int32>	Arr_NavLink_Index;
	TArray<int32>	Arr_NavLink_TargetTileIndex;
	TArray<int32>	Arr_TileIndex_TargetThis;	//Array of Tile index that Targeting this tile index

};
