#pragma once

#include "EngineMinimal.h"
#include "HHM_Parameter_PathFind.generated.h"

USTRUCT(BlueprintType)
struct FHHM_Parameter_PathFind
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Param_Pathfind)
		int32	EntitySize_Width = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Param_Pathfind)
		int32	EntitySize_Height = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Param_Pathfind)
		int32	MaxJumpLength = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Param_Pathfind)
		int32	MaxFallLength = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Param_Pathfind)
		int32	MaxHorizontalJumpLength = 0;

public:
	FHHM_Parameter_PathFind() {

	}

	FHHM_Parameter_PathFind(int32 _entitySize_Width, int32 _entitySize_Height, int32 _maxJumpLength, int32 _maxFallLength, int32 _maxHorizontalJumpLength) {
		EntitySize_Width = _entitySize_Width;
		EntitySize_Height = _entitySize_Height;

		MaxJumpLength = _maxJumpLength;
		MaxFallLength = _maxFallLength;
		MaxHorizontalJumpLength = _maxHorizontalJumpLength;
	}
};

//, int32 _entitySize_Width, int32 _entitySize_Height, int32 _maxJumpLength, int32 _maxFallLength, int32 _maxHorizontalJumpLength = 1