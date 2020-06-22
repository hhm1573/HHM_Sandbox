#pragma once

#include "CoreMinimal.h"

#include "Header/Struct.h"

#include "Struct_LocalMap.generated.h"

USTRUCT()
struct FHHM_LocalMapConstructionData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConstructionData_Base")
		FHHM_MapInfo	MapInfo;

};



USTRUCT()
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