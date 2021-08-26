#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"

UENUM(BlueprintType)
enum class EHHM_MovementMode : uint8
{
	Move_OnGround		UMETA(DisplayName = "Move_OnGround"),
	Move_Falling		UMETA(DisplayName = "Move_Falling"),
	Move_End			UMETA(Hidden)
};