#pragma once

#include "EngineMinimal.h"

#include "Header/Struct_Renderer.h"

#include "HHM_Container_SubTileRenderInstance.generated.h"

USTRUCT()
struct FHHM_Container_SubTileRenderInstance
{
	GENERATED_USTRUCT_BODY();

public:
	TMap<int32, FHHM_InstancedMeshArray> Container_SubTileRenderInstance;

public:
	FHHM_Container_SubTileRenderInstance() {
		Container_SubTileRenderInstance.Empty();
	}
};