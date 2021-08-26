#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"

UENUM(BlueprintType)
enum class EHHM_AI_PathState : uint8
{
	PathState_Begin			UMETA(DisplayName = PathState_PathBegin),
	PathState_Following		UMETA(DisplayName = PathState_Following),
	PathState_Success		UMETA(DisplayName = PathState_Success),
	PathState_NoPath		UMETA(DisplayName = PathState_NoPath),				//Can not reach to target location. no valid path found.
	PathState_Blocked		UMETA(DisplayName = PathState_Blocked),				//Path got blocked during following path.
	PathState_OffPath		UMETA(DisplayName = PathState_OffPath),				//got off from the following path. for example by falling or got launched
	PathState_Abort			UMETA(DisplayName = PathState_Abort),
	PathState_Errored		UMETA(DisplayName = PathState_Errored),
	PathState_End			UMETA(Hidden)
};