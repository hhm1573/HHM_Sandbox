#pragma once

#include "CoreMinimal.h"

#include "Header/Struct.h"
#include "Header/Struct_Tile.h"

#include "Header/Enum_LocalMap.h"

#include "TileEntity/HHM_TileEntity.h"

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

public:
	FHHM_LocalMapConstructionData() {
		MapInfo = FHHM_MapInfo();
		GeneratorType = EHHM_LocalMap_Option_Generator::LocalMapGenerator_Empty;
	}

	FHHM_LocalMapConstructionData(const FHHM_MapInfo& _mapInfo, const EHHM_LocalMap_Option_Generator& _generatorType) {
		MapInfo = _mapInfo;
		GeneratorType = _generatorType;
	}
};



USTRUCT(BlueprintType)
struct FHHM_LocalMapConstructionResult
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConstructionResult")
		int32	MapIndex = -1;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConstructionResult")
		int32	MapIndex_Horizontal = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConstructionResult")
		int32	MapIndex_Vertical = -1;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConstructionResult")
		class ALocalMap*	pLocalMap = nullptr;

};



USTRUCT(BlueprintType)
// HHM Note : Make it as class. also make property class that can store data as string (serialize)
// needed functions : Add Remove Resize Num etc.. make it can handle multiple arrays at once
struct FHHM_LocalMap_MapData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapData")
		TArray<FHHM_TileData>				Container_TileData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapData")
		TArray<FHHM_TileMovementInfo>		Container_TileMovementData;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapData")
		//TArray<TSharedPtr<AHHM_TileEntity>>	Container_TileEntity;
		TArray<AHHM_TileEntity*>	Container_TileEntity;

public:
	void Empty(void) { Container_TileData.Empty(); Container_TileMovementData.Empty(); Container_TileEntity.Empty(); }
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