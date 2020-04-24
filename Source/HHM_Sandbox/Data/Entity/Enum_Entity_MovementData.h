#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"

UENUM(BlueprintType)
enum class EHHM_Entity_Movement_State : uint8
{
	MoveState_Begin		UMETA(DisplayName = "MoveState_Begin"),
	MoveState_During	UMETA(DisplayName = "MoveState_During"),
	MoveState_End		UMETA(DisplayName = "MoveState_End"),
	MoveState_Dummy		UMETA(DisplayName = "MoveState_Dummy")
};