#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"

UENUM(BlueprintType)
enum class EHHM_RenderType : uint8
{
	RType_Instanced		UMETA(DisplayName = "RenderType_Instanced"),
	RType_Independent		UMETA(DisplayName = "RenderType_Independent"),
	RType_Unavailiable		UMETA(DisplayName = "RenderType_Unavailiable")
};