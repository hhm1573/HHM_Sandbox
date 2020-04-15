#pragma once

#include "CoreMinimal.h"
#include "Struct_Entity_MovementData.generated.h"

USTRUCT()
struct FHHM_Entity_MovementData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
		float	MoveSpeed_Default = 0.0f;

	UPROPERTY()
		int32	Jump_Vertical_MaxHeight = 0;
	UPROPERTY()
		int32	Jump_Vertical_FreeJump_Minimum = 0;
	UPROPERTY()
		//How much time needs before start jump
		float	Jump_Vertical_FreeJump_Duration_Start = 0.0f;
	UPROPERTY()
		//How much time needs before end jump
		float	Jump_Vertical_FreeJump_Duration_End = 0.0f;
	UPROPERTY()
		//How fast entity will rise during jump. Tile per Sec
		float	Jump_Vertical_FreeJump_Speed = 0.0f;
	UPROPERTY()
		//Container of animation durations
		//When entity do jump(except free-jump), entity will teleport to target location after duration time.
		//index over 'FreeJump_Minimum' will be ignored and might cause some issue.
		TArray<float>	Jump_Vertical_AnimationDuration;

	UPROPERTY()
		int32	Fall_MaxHeight = 0;

	UPROPERTY()
		int32	Jump_Horizontal_MaxLength = 0;
	UPROPERTY()
		TArray<float>	Jump_Horizontal_AnimationDuration;


};



USTRUCT()
struct FHHM_Movement_SpeedMultiplicationData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
		//Higher weight element will be multiplied more lately
		int32	Weight = 0;
	UPROPERTY()
		float	MultiplyValue = 0.0f;
};