// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"

#include "Header/Struct_Pathfinder.h"
#include "Data/Entity/Struct_Entity_MovementData.h"
#include "Data/Entity/Enum_Entity_MovementData.h"

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

	//Temporary function
public:
	void Temp_Set_MoveData(FHHM_Entity_MovementData& _moveData);// { m_MovementData = _moveData; }
	void Temp_Set_DefaultSpeed(float _speed_TilePerSec) { m_Speed_Default = _speed_TilePerSec; }

protected:
	UPROPERTY()
		float			m_TileSize = 0.0f;

	UPROPERTY()
		FHHM_Entity_MovementData	m_MovementData = FHHM_Entity_MovementData();



	UPROPERTY()
		bool						m_bIsFalling = false;
	UPROPERTY()
		bool						m_bIsFalling_Before = false;

	UPROPERTY()
		bool						m_bIsRecovering = false;



	UPROPERTY()
		EHHM_MoveType				m_MoveType_Current = EHHM_MoveType::MT_OnGround;
	UPROPERTY()
		EHHM_MoveType				m_MoveType_Before = EHHM_MoveType::MT_OnGround;
	UPROPERTY()
		EHHM_Entity_Movement_State	m_MoveState_Current = EHHM_Entity_Movement_State::MoveState_Dummy;
	//UPROPERTY()
	//	EHHM_Entity_Movement_State	m_MoveState_Before = EHHM_Entity_Movement_State::MoveState_Dummy;	// HHM Note : this variable might not needed
	UPROPERTY()
		//Used for moves like jump,JumpDown,etc.. when timer reached to zero, character will move to target location
		float						m_Move_Timer = 0.0f;
	UPROPERTY()
		FVector						m_MoveTarget_Current = FVector();
	


	UPROPERTY()
		TArray<float>									m_Container_Speed_ModificationElement;
	UPROPERTY()
		TArray<FHHM_Movement_SpeedMultiplicationData>	m_Container_Speed_MultiplicationElement;



	UPROPERTY()
	float m_Speed_Default = 0.0f; // Tile Per Sec
	UPROPERTY()
	float m_Speed_Current = 0.0f; //Tile Per Sec
	


	UPROPERTY()
	TArray<FHHM_PathNodeData>	m_FollowingPath;



	UPROPERTY()
		int32 m_EntitySize_Horizontal = 1;
	UPROPERTY()
		int32 m_EntitySize_Vertical = 1;

	UPROPERTY()
		int32 m_MaxJumpHeight = 0;
	UPROPERTY()
		int32 m_MaxJumpDownHeight = 0;
	UPROPERTY()
		int32 m_MaxHorizontalJumpLength = 0;

	UPROPERTY()
		float m_MoveDistance_ThisTick = 0.0f;
	UPROPERTY()
		float m_MoveDistance_PostTick = 0.0f;


public:
	virtual void	BeginPlay() override;
	virtual void	TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION()
	bool			MoveToLocation(int32 _index_Horizontal, int32 _index_Vertical);

	UFUNCTION()
		bool		Change_Face_Direction(bool _toLeft = true);

#pragma region Getter
	//Get speed etc.

public:
	UFUNCTION(BlueprintPure)
		float		Get_MoveTimer() { return m_Move_Timer; }

	UFUNCTION(BlueprintPure)
		bool			Get_IsFalling() { return m_bIsFalling; }
	UFUNCTION(BlueprintPure)
		bool			Get_IsFalling_Before() { return m_bIsFalling_Before; }

	UFUNCTION(BlueprintPure)
		bool			Get_IsRecovering() { return m_bIsRecovering; }

	UFUNCTION(BlueprintPure)
		int32			Get_MoveValue();			//Get MoveValue. when doing jump, move value is bigger as one than actual jump length. Actual Jump Length = MoveValue - 1

	UFUNCTION(BlueprintPure)
		EHHM_MoveType	Get_MoveType_Current() { return m_MoveType_Current; }
	UFUNCTION(BlueprintPure)
		EHHM_MoveType	Get_MoveType_Before() { return m_MoveType_Before; }

	UFUNCTION()
		FVector			Get_Location();				//Get Location Of UpdatedComponent of UMovementComponent
	UFUNCTION()
		FVector			Get_Location_BottomLeft();	//Get Bottom-Center Location of Bottom_Left Entity-Tile

	UFUNCTION()
		FRotator		Get_Rotation_Safe();			//Get Rotation of Updated Component of UMovementComponent. Do Safety check

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
	//Called when entity is falling. called on TickComponent function
	virtual void	Update_Falling(float DeltaTime);
	//Called when entity is recovering from falling or etc. called on TickComponent function
	virtual void	Update_Recovering(float DeltaTime);
	virtual void	FollowPath(float DeltaTime);

#pragma region Implement_FollowingPath
private:
	void			FollowPath_Jump(float DeltaTime);
	void			FollowPath_Jump_Free(float DeltaTime);

	void			FollowPath_DownJump(float DeltaTime);
	void			FollowPath_DownJump_Free(float DeltaTime);

	void			FollowPath_HorizontalJump(float DeltaTime);
	void			FollowPath_HorizontalJump_Free(float DeltaTime);

	void			FollowPath_Walk(float DeltaTime/*, const FVector& _location_Current, const FVector& _location_Target*/);
#pragma endregion

private:
	bool			Calculate_MoveTarget_Location(void);

	bool			Check_IsBelowFloor_Standable(void);

	void			Abort_Path(void);
	
	
};
