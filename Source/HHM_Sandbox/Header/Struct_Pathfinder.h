#pragma once

#include "CoreMinimal.h"
#include "Struct_Pathfinder.generated.h"



UENUM(BlueprintType)
enum class EHHM_MoveType : uint8
{
	MT_OnGround					UMETA(DisplayName = "OnGround"),
	MT_Jump						UMETA(DisplayName = "Jump"),
	MT_Fall						UMETA(DisplayName = "Fall"),
	MT_HorizontalJump_Left		UMETA(DisplayName = "HorizontalJump_Left"),
	MT_HorizontalJump_Right		UMETA(DisplayName = "HorizontalJump_Right"),
	MT_End						UMETA(DisplayName = "End(Dummy)")
};



USTRUCT()
struct FHHM_PathFinderNode
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Node_Info")
		FVector2D		Location_Parent = FVector2D(-1.0f, -1.0f);
		EHHM_MoveType	eMoveType_Parent = EHHM_MoveType::MT_OnGround;


	UPROPERTY(VisibleAnywhere, Category = "Node_Info")
		float			Cost_G = 0.0f;	//Distance from Start node
	UPROPERTY(VisibleAnywhere, Category = "Node_Info")
		float			Cost_H = 0.0f;	//Heuristic cost (default : distance from end)
	UPROPERTY(VisibleAnywhere, Category = "Node_Info")
		float			Cost_F = 0.0f;	//Sum of G and H Costs
	UPROPERTY(VisibleAnywhere, Category = "Node_Info")
		int8			JumpValue = 0;
	UPROPERTY(VisibleAnywhere, Category = "Node_Info")
		int8			HorizontalJumpValue = 0;
	/*UPROPERTY(VisibleAnywhere, Category = "Node_Info")
		EHHM_MoveType	eMoveType = EHHM_MoveType::MT_OnGround;*/
	UPROPERTY(VisibleAnywhere, Category = "Node_Info")
		bool			MoveOnAir = false;

	UPROPERTY(VisibleAnywhere, Category = "Node_Info")
		int8		Status = 0;
	//Note : it can be optimized if memory is not enough
};



USTRUCT()
struct FHHM_PathFinderNodeInfo	//it used for simplifying open set on path-finding process
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Node_Info")
		int32			Index = 0;
		EHHM_MoveType	eMoveType = EHHM_MoveType::MT_OnGround;

	UPROPERTY(VisibleAnywhere, Category = "Node_Info")
		float			Cost_F = 0.0f;

public:
	FHHM_PathFinderNodeInfo() {

	}

	FHHM_PathFinderNodeInfo(int32 _index, EHHM_MoveType _eMoveType, float _cost_F) {
		Index = _index;
		eMoveType = _eMoveType;
		Cost_F = _cost_F;
	}
};



USTRUCT(Blueprintable)
struct FHHM_PathNodeData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PathNodeData_TargetTileIndex")
		int32			Index_Target = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PathNodeData_StartTileIndex")
		int32			Index_Start = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PathNodeData_MoveType")
		EHHM_MoveType	eMoveType = EHHM_MoveType::MT_OnGround;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PathNodeData_MoveValue")
		int32			MoveValue = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PathNodeData_Location")
		FVector2D		Location = FVector2D(-1.0f, -1.0f);

public:
	FHHM_PathNodeData() {

	}

	FHHM_PathNodeData(int32 _index_Start, int32 _index_Target , EHHM_MoveType _eMoveType, int32 _moveValue, FVector2D& _location) {
		Index_Target = _index_Target;
		Index_Start = _index_Start;
		eMoveType = _eMoveType;
		MoveValue = _moveValue;
		Location = _location;
	}

};