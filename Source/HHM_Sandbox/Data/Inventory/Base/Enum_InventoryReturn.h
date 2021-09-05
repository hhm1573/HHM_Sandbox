#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"

UENUM(BlueprintType)
enum class EHHM_InventoryReturn : uint8
{
	Return_Succeed		UMETA(DisplayName = "Return_Succeed"),
	Return_NoRoom		UMETA(DisplayName = "Return_NoRoom"),
	Return_NoItem		UMETA(DisplayName = "Return_NoItem"),
	Return_Error		UMETA(DisplayName = "Return_Error"),
	Return_End			UMETA(DisplayName = "Return_End")
};
