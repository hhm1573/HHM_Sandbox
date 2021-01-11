#pragma once

#include "CoreMinimal.h"
#include "HHM_RenderData_Item.generated.h"

USTRUCT(BlueprintType)
struct FHHM_RenderData_Item
{
	GENERATED_USTRUCT_BODY()

public:
	TArray<class UMaterialInterface*>	Container_Material;
	FVector2D							Size_ItemActor;
	
public:
	FHHM_RenderData_Item() {
		Container_Material.Empty();
		Size_ItemActor = FVector2D::ZeroVector;
	}

	//머티리얼배열, 2D벡터, 인벤사이즈?		인벤사이즈는 렌더보다는 인벤토리에 관련이라 안넣음.
};