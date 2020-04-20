// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"

#include "Header/Struct_Pathfinder.h"
#include "Data/Entity/Struct_Entity_MovementData.h"

#include "HHM_Component_Movement.generated.h"

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API UHHM_Component_Movement : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	UHHM_Component_Movement();

protected:
	UPROPERTY()
		float			m_TileSize = 0.0f;

	UPROPERTY()
		FHHM_Entity_MovementData	m_MovementData = FHHM_Entity_MovementData();



	UPROPERTY()
		EHHM_MoveType	m_MoveType_Current = EHHM_MoveType::MT_OnGround;
	UPROPERTY()
		//Used for moves like jump,fall,etc.. when timer reached to zero, character will move to target location
		float			m_Move_Timer = 0.0f;
	UPROPERTY()
		FVector			m_MoveTarget_Current = FVector();
	UPROPERTY()
		EHHM_MoveType	m_MoveType_Before = EHHM_MoveType::MT_OnGround;

	UPROPERTY()
		TArray<float>									m_Container_Speed_ModificationElement;
	UPROPERTY()
		TArray<FHHM_Movement_SpeedMultiplicationData>	m_Container_Speed_MultiplicationElement;

	int32 m_EntitySize_Horizontal = 1;
	int32 m_EntitySize_Vertical = 1;

	int32 m_MaxJumpHeight = 0;
	int32 m_MaxFallHeight = 0;
	int32 m_MaxHorizontalJumpLength = 0;

	float m_Speed_Default = 0.0f; // Tile Per Sec
	float m_Speed_Current = 0.0f; //Tile Per Sec

	float m_MoveDistance_ThisTick = 0.0f;
	float m_MoveDistance_PostTick = 0.0f;

	TArray<FHHM_PathNodeData>	m_FollowingPath;

public:
	virtual void	BeginPlay() override;
	virtual void	TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	bool			MoveToLocation(int32 _index_Horizontal, int32 _index_Vertical);

#pragma region Getter

	

#pragma endregion

public:
	//Added element will be summed with speed value.
	//* Return value : index of value added on container. return negative value if adding work failed
	int32			Add_Speed_ModificationElement(float _modifyValue);

	//Added element will be multiplied on speed value. Multiplying job will be done after all Modification Elements applied.
	//Higher weight value will be multiplied more later.
	//* Return value : index of value added on container. return negative value if adding work failed
	int32			Add_Speed_MultiplicationElement(int32 _weight, float _multiplyValue);

#pragma region State_Check
public:
	bool			Is_OnAir_Begin(void);
	bool			Is_OnAir(void);
	bool			Is_OnAir_End(void);

	bool			IsJumpStart(void);
	bool			IsJumping(void);
	bool			IsJumpEnd(void);
#pragma endregion

protected:
	virtual void	Update_MovementSpeed(float DeltaTime);
	virtual void	FollowPath(float DeltaTime);

#pragma region Implement_FollowingPath
private:
	void			FollowPath_Jump(float DeltaTime);
	void			FollowPath_Jump_Free(float DeltaTime);

	void			FollowPath_Fall(float DeltaTime);
	void			FollowPath_Fall_Free(float DeltaTime);

	void			FollowPath_HorizontalJump(float DeltaTime);
	void			FollowPath_HorizontalJump_Free(float DeltaTime);

	void			FollowPath_Walk(float DeltaTime/*, const FVector& _location_Current, const FVector& _location_Target*/);
#pragma endregion
	
	
};
