#pragma once

#include "CoreMinimal.h"

#include "Header/Struct.h"
#include "Header/Struct_Tile.h"

#include "Header/Enum_LocalMap.h"

#include "Struct_LocalMap.generated.h"

USTRUCT(BlueprintType)
struct FHHM_LocalMapConstructionData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConstructionData_Base")
		FHHM_MapInfo	MapInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConstructionData_Base")
		EHHM_LocalMap_Option_Generator		GeneratorType;

};



USTRUCT(BlueprintType)
struct FHHM_LocalMapConstructionResult
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConstructionResult")
		int32	MapIndex = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConstructionResult")
		int32	MapIndex_Horizontal = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConstructionResult")
		int32	MapIndex_Vertical = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConstructionResult")
		class ALocalMap*	pLocalMap = nullptr;

};



USTRUCT(BlueprintType)
struct FHHM_LocalMap_MapData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapData")
		TArray<FHHM_TileData>			Container_TileData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapData")
		TArray<FHHM_TileMovementInfo>	Container_TileMovementData;
};




//USTRUCT()
//struct FHHM_Wrapper_LocalMap
//{
//	GENERATED_USTRUCT_BODY()
//
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		bool
//
//};