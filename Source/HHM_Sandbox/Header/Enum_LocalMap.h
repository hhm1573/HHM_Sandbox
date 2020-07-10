#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"

UENUM(BlueprintType)
enum class EHHM_LocalMap_Option_Generator : uint8
{
	LocalMapGenerator_Empty			UMETA(DisplayName = "Generator_Empty"),
	LocalMapGenerator_TestMap		UMETA(DisplayName = "Generator_TestMap")
};