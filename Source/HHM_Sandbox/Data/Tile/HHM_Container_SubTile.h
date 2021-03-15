#pragma once

#include "EngineMinimal.h"
#include "HHM_Container_SubTile.generated.h"

USTRUCT()
struct FHHM_Container_SubTile
{
	GENERATED_USTRUCT_BODY();

public:
	TMap<int32, class AHHM_Tile*> Container_SubTile;

public:
	FHHM_Container_SubTile() {
		Container_SubTile.Empty();
	}
};