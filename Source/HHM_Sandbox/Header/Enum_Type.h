#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"

UENUM(BlueprintType)
enum class EHHM_DataType : uint8
{
	Type_Bool				UMETA(DisplayName = "Type_Bool"),
	Type_Int				UMETA(DisplayName = "Type_Int"),
	Type_Float				UMETA(DisplayName = "Type_Float"),
	Type_String				UMETA(DisplayName = "Type_String"),
	Type_Bundle				UMETA(DisplayName = "Type_Bundle")
};