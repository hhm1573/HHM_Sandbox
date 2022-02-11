#pragma once

#include "CoreMinimal.h"

#include "Header/Enum_RenderType.h"

#include "Struct_RenderInfo.generated.h"

USTRUCT(BlueprintType)
struct FHHM_RenderInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileRenderData")	//LocalMap Does not get involved to render if it is true
		bool				IsIndependent = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileRenderData")
		TArray<class UMaterialInterface*>	Arr_Material;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileRenderData")
		EHHM_RenderType		eRenderType = EHHM_RenderType::RType_Unavailiable;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileRenderData")
		int32				Num_Material = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileRenderData")
		bool				IsNeedToRender = true;
};



USTRUCT(BlueprintType)
struct FHHM_RenderInfo_ItemActor
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemActor_RenderData")
		class UMaterialInterface*	Material;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemActor_RenderData")
		FVector2D					CubeSize = FVector2D::ZeroVector;
};