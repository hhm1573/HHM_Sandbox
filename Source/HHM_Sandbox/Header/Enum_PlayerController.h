#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"

UENUM(BlueprintType)
enum class EHHM_ControllerMode : uint8
{
	ControllerMode_Selection	UMETA(DisplayName = Mode_Selection),
	ControllerMode_Order		UMETA(DisplayName = Mode_Order),
	ControllerMode_Plan			UMETA(DisplayName = Mode_Plan),
	ControllerMode_Blueprint	UMETA(DisplayName = Mode_Blueprint),
	ControllerMode_End			UMETA(DisplayName = Mode_Dummy)
};
//선택모드 (조사모드)
//명령모드 (명령 타입 필요. 위치대상? 엔티티 대상? 특정 건축물 대상?)
//계획모드 (채굴, 아이템 이동 등의 명령이나 건축계획)
//건설모드 (블루프린트 설치)