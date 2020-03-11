#pragma once

#include "CoreMinimal.h"
#include "Struct_Renderer.generated.h"

USTRUCT(BlueprintType)
struct FHHM_InstancedMeshArray
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere)
		TArray<class UInstancedStaticMeshComponent*> Arr_pInstancedStaticMesh;
};



USTRUCT()
struct FHHM_RenderData {
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere)
		int32		Tile_ID = 0;
	UPROPERTY(VisibleAnywhere)
		int32		Tile_SubID = 0;
	UPROPERTY(VisibleAnywhere)
		int32		Render_Index = -1;

public:
	FORCEINLINE FHHM_RenderData() {
		Tile_ID = 0;
		Render_Index = -1;
	}

	FORCEINLINE FHHM_RenderData(int32 TileID, int32 TileSubID) :Tile_ID(TileID), Tile_SubID(TileSubID) {
		Render_Index = -1;
	}



	//public:
	//	FORCEINLINE FHHM_TileRenderInfo() {
	//		Tile_ID = 0;
	//		Render_Index = -1;
	//	}
	//
	//	FORCEINLINE FHHM_TileRenderInfo(int32 TileID) : Tile_ID(TileID) {
	//		Render_Index = -1;
	//	}
};