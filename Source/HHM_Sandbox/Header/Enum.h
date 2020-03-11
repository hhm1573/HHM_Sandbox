#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"

UENUM(BlueprintType)
enum class EHHM_DamageType : uint8
{
	Damage_Melee		UMETA(DisplayName = "Damage_Melee"),
	Damage_Range		UMETA(DisplayName = "Damage_Range"),
	Damage_Explosion	UMETA(DisplayName = "Damage_Explosion"),
	Damage_Unavailiable	UMETA(DisplayName = "Damage_Unavailiable")
};