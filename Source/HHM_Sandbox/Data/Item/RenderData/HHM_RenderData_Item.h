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

	//��Ƽ����迭, 2D����, �κ�������?		�κ�������� �������ٴ� �κ��丮�� �����̶� �ȳ���.
};