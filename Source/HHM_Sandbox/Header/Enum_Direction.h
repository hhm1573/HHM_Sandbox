#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"

UENUM(BlueprintType)
enum class EHHM_Direction4 : uint8
{
	Dir_Up				UMETA(DisplayName = "Direction_Up"),
	Dir_Down			UMETA(DisplayName = "Direction_Down"),
	Dir_Left			UMETA(DisplayName = "Direction_Left"),
	Dir_Right			UMETA(DisplayName = "Direction_Right")
};