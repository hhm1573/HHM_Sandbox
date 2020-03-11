#pragma once

#include "CoreMinimal.h"

#include "Header/Enum_RenderType.h"

#include "Struct_RenderInfo.generated.h"

USTRUCT(BlueprintType)
struct FHHM_RenderInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileRenderData")
		TArray<class UMaterialInterface*>	Arr_Material;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileRenderData")
		EHHM_RenderType		eRenderType = EHHM_RenderType::RType_Unavailiable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileRenderData")
		int32				Num_Material = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileRenderData")
		bool				IsNeedToRender = true;
};