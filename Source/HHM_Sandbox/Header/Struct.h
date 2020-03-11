#pragma once

#include "CoreMinimal.h"
#include "Struct.generated.h"

USTRUCT(BlueprintType)
struct FHHM_MapInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapInfo")
		int32		MapSize_Horizontal = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapInfo")
		int32		MapSize_Vertical = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapInfo")
		float		TileSize = 0.0f;
};



//USTRUCT(BlueprintType)
//struct FHHM_TileInfo {
//	GENERATED_USTRUCT_BODY()
//
//public:
//	UPROPERTY()
//		int32		Tile_RenderID = 0;
//	UPROPERTY()
//		int32		Tile_HealthPoint = 0;
//	UPROPERTY()
//		int32		Tile_HealthPoint_Max = 0;
//	UPROPERTY()
//		bool		IsBlockPassage = false;
//};



