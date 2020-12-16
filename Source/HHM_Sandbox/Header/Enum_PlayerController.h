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
//���ø�� (������)
//��ɸ�� (��� Ÿ�� �ʿ�. ��ġ���? ��ƼƼ ���? Ư�� ���๰ ���?)
//��ȹ��� (ä��, ������ �̵� ���� ����̳� �����ȹ)
//�Ǽ���� (�������Ʈ ��ġ)