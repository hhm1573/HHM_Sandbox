// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Component_Movement.h"
#include "Header/Struct_Pathfinder.h"

#include "Engine/World.h"
#include "Base/GameMode/HHM_GameMode_LocalMap.h"
#include "Manager/Navigation/HHM_Manager_Navigation.h"
#include "Data/LocalMap/LocalMap.h"
#include "Header/Struct_Pathfinder.h"

#include "Manager/Math/Grid/HHM_Manager_Math_Grid.h"



UHHM_Component_Movement::UHHM_Component_Movement() {

}

void UHHM_Component_Movement::BeginPlay() {
	Super::BeginPlay();

	m_FollowingPath.Empty();



	//Set TileSize Value
	UWorld* pWorld = nullptr;
	pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception
		return;
	}

	AGameModeBase* pGameMode_Raw = nullptr;
	pGameMode_Raw = pWorld->GetAuthGameMode();
	if (pGameMode_Raw == nullptr) {
		//Exception
		return;
	}

	AHHM_GameMode_LocalMap* pGameMode = nullptr;
	pGameMode = Cast<AHHM_GameMode_LocalMap>(pGameMode_Raw);
	if (pGameMode == nullptr) {
		//Exception
		return;
	}

	const FHHM_MapInfo& Ref_MapInfo = pGameMode->Get_MapInfo();
	m_TileSize = Ref_MapInfo.TileSize;
}

void UHHM_Component_Movement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) {
		return;
	}



	//Update MoveType
	int32 Num_FollowingPath = m_FollowingPath.Num();
	m_MoveType_Before = m_MoveType_Current;
	if (Num_FollowingPath > 0) {
		m_MoveType_Current = m_FollowingPath[0].eMoveType;
	}
	else {
		m_MoveType_Current = EHHM_MoveType::MT_OnGround;
		return;
	}



	UHHM_Component_Movement::Update_MovementSpeed(DeltaTime);
	


	if (m_FollowingPath.Num() > 0) {
		FollowPath(DeltaTime);
	}
}

bool UHHM_Component_Movement::MoveToLocation(int32 _index_Horizontal, int32 _index_Vertical) {
	// HHM Note : Add Location Check whether entity can reach/stand or not

	UWorld* pWorld = GetWorld();
	if (pWorld == nullptr) {
		return false;
	}

	AGameModeBase* pGameMode_Raw = pWorld->GetAuthGameMode();
	if (pGameMode_Raw == nullptr) {
		return false;
	}

	AHHM_GameMode_LocalMap* pGameMode = Cast<AHHM_GameMode_LocalMap>(pGameMode_Raw);
	if (pGameMode == nullptr) {
		return false;
	}

	const AHHM_Manager_Navigation* pManager_Navigation = pGameMode->Get_Manager_Navigation_Const();
	if (pManager_Navigation == nullptr) {
		return false;
	}

	const ALocalMap* pLocalMap = pGameMode->Get_LocalMap_Const();
	if (pLocalMap == nullptr) {
		return false;
	}



	FVector Vec_Location_Current = GetActorLocation();
	FVector2D Vec_Location_Start = FVector2D();
	if (pGameMode->Utility_Calculate_IndexLocation(Vec_Location_Start, Vec_Location_Current.X, Vec_Location_Current.Z) == false) {
		return false;
	}
	FVector2D Vec_Location_End = FVector2D(_index_Horizontal, _index_Vertical);

	TArray<FHHM_PathNodeData> Path_Find = pManager_Navigation->Search_Path(pLocalMap, Vec_Location_Start, Vec_Location_End, m_EntitySize_Horizontal, m_EntitySize_Vertical, m_MovementData.Jump_Vertical_MaxHeight, m_MovementData.Fall_MaxHeight, m_MovementData.Jump_Horizontal_MaxLength);
	//pManager_Navigation->Search_Path(pLocalMap, )
	m_FollowingPath = Path_Find;

	bool IsSucceed_Calculation = UHHM_Component_Movement::Calculate_MoveTarget_Location();
	if (IsSucceed_Calculation == false) {
		m_FollowingPath.Empty();
		return false;
	}

	

	return true;
}



int32	UHHM_Component_Movement::Add_Speed_ModificationElement(float _modifyValue) {
	return 0;
}

int32	UHHM_Component_Movement::Add_Speed_MultiplicationElement(int32 _weight, float _multiplyValue) {
	return 0;
}

#pragma region State_Check

bool UHHM_Component_Movement::Is_OnAir_Begin(void) {
	bool IsCurrentlyOnAir = m_MoveType_Current == EHHM_MoveType::MT_Jump || m_MoveType_Current == EHHM_MoveType::MT_Fall ||
		m_MoveType_Current == EHHM_MoveType::MT_HorizontalJump_Left || m_MoveType_Current == EHHM_MoveType::MT_HorizontalJump_Right ? true : false;

	return IsCurrentlyOnAir == true && m_MoveType_Before == EHHM_MoveType::MT_OnGround ? true : false;
}

bool UHHM_Component_Movement::Is_OnAir(void) {
	bool IsCurrentlyOnAir = m_MoveType_Current == EHHM_MoveType::MT_Jump || m_MoveType_Current == EHHM_MoveType::MT_Fall ||
		m_MoveType_Current == EHHM_MoveType::MT_HorizontalJump_Left || m_MoveType_Current == EHHM_MoveType::MT_HorizontalJump_Right ? true : false;

	bool IsOnAirBefore = m_MoveType_Before == EHHM_MoveType::MT_Jump || m_MoveType_Before == EHHM_MoveType::MT_Fall ||
		m_MoveType_Before == EHHM_MoveType::MT_HorizontalJump_Left || m_MoveType_Before == EHHM_MoveType::MT_HorizontalJump_Right ? true : false;

	return IsCurrentlyOnAir == true && IsOnAirBefore == true ? true : false;
}

bool UHHM_Component_Movement::Is_OnAir_End(void) {
	bool IsOnAirBefore = m_MoveType_Before == EHHM_MoveType::MT_Jump || m_MoveType_Before == EHHM_MoveType::MT_Fall ||
		m_MoveType_Before == EHHM_MoveType::MT_HorizontalJump_Left || m_MoveType_Before == EHHM_MoveType::MT_HorizontalJump_Right ? true : false;

	return IsOnAirBefore == true && m_MoveType_Current == EHHM_MoveType::MT_OnGround ? true : false;
}



bool UHHM_Component_Movement::IsJumpStart(void) {
	return m_MoveType_Current == EHHM_MoveType::MT_Jump && m_MoveType_Before == EHHM_MoveType::MT_OnGround ? true : false;
}

bool UHHM_Component_Movement::IsJumping(void) {
	return m_MoveType_Current == EHHM_MoveType::MT_Jump && m_MoveType_Before == EHHM_MoveType::MT_Jump ? true : false;
}

bool UHHM_Component_Movement::IsJumpEnd(void) {
	return m_MoveType_Current == EHHM_MoveType::MT_OnGround && m_MoveType_Before == EHHM_MoveType::MT_Jump ? true : false;
}

#pragma endregion



void UHHM_Component_Movement::Update_MovementSpeed(float DeltaTime) {
	float Speed_Current_Tick = m_Speed_Default;



	//Applying modification elements
	int32 Num_ModificationElements = m_Container_Speed_ModificationElement.Num();

	if (Num_ModificationElements > 0) {
		for (int32 Index = 0; Index < Num_ModificationElements; ++Index) {
			float ModificationValue = m_Container_Speed_ModificationElement[Index];
			Speed_Current_Tick += ModificationValue;
		}
	}



	//Applying Multiplication Elements
	float Speed_Before_Multiply = Speed_Current_Tick;

	int32 Num_MultiplicationElements = m_Container_Speed_MultiplicationElement.Num();

	//Sorting Multiplication Elements
	if (Num_MultiplicationElements > 0) {
		TArray<FHHM_Movement_SpeedMultiplicationData> Container_Speed_MultiplicationElement_Sorted;
		Container_Speed_MultiplicationElement_Sorted = m_Container_Speed_MultiplicationElement;

		// Note :: here sorting job is done by simple bubble sorting. change it to better sorting algorithm if any performance issue occurs
		for (int32 SortingIndex = Num_MultiplicationElements - 1; SortingIndex > 0; --SortingIndex) {
			for (int32 CurrentSortingIndex = 0; CurrentSortingIndex < SortingIndex; ++CurrentSortingIndex) {
				int32 Index_Compare = CurrentSortingIndex + 1;

				int32 Weight_Current = m_Container_Speed_MultiplicationElement[CurrentSortingIndex].Weight;
				int32 Weight_Compare = m_Container_Speed_MultiplicationElement[Index_Compare].Weight;

				if (Weight_Compare > Weight_Current) {
					float MultiplyValue_Current = m_Container_Speed_MultiplicationElement[CurrentSortingIndex].MultiplyValue;
					float MultiplyValue_Compare = m_Container_Speed_MultiplicationElement[Index_Compare].MultiplyValue;
					
					m_Container_Speed_MultiplicationElement[CurrentSortingIndex] = FHHM_Movement_SpeedMultiplicationData(MultiplyValue_Compare, Weight_Compare);
					m_Container_Speed_MultiplicationElement[Index_Compare] = FHHM_Movement_SpeedMultiplicationData(MultiplyValue_Current, Weight_Current);
				}
			}
		}
	}

	for (int32 ApplyingIndex = 0; ApplyingIndex < Num_MultiplicationElements; ++ApplyingIndex) {
		float Speed_Before_Applying = Speed_Current_Tick;
		float Speed_After_Applying = Speed_Before_Applying * m_Container_Speed_MultiplicationElement[ApplyingIndex].MultiplyValue;

		Speed_Current_Tick = Speed_After_Applying;
	}



	//Clear Speed Modifying Elements
	m_Container_Speed_ModificationElement.Empty();
	m_Container_Speed_MultiplicationElement.Empty();



	m_Speed_Current = Speed_Current_Tick;
}



void UHHM_Component_Movement::FollowPath(float DeltaTime) {

	FVector Location_Current = GetActorLocation();
	//float	Location_Start_X = Location_Current.X - ((m_EntitySize_Horizontal - 1) * (m_TileSize * 0.5f));
	//float	Location_Start_Y = Location_Current.Y - ((m_EntitySize_Vertical - 1) * (m_TileSize * 0.5f));
	//FVector Location_Start = FVector(Location_Start_X, 0.0f, Location_Start_Y);														//HHM Note : Y Level Control
	//FVector2D IndexLocation_Start = FVector2D(Location_Start.X / m_TileSize, Location_Start.Z / m_TileSize);



	EHHM_MoveType MoveType_Current = m_FollowingPath[0].eMoveType;



	//Check is entity at target point
	FVector Vec_CurrentToTarget = m_MoveTarget_Current - Location_Current;
	float	Distance_Target = Vec_CurrentToTarget.Size();
	//HHM Note : Change this Value later ---- ---- ---- ----
	float	Acceptable_Distance = 10.0f;

	//Return if entity is already at target point
	if (Distance_Target <= Acceptable_Distance) {
		m_FollowingPath.RemoveAt(0);

		bool IsSucceed_Calculation = UHHM_Component_Movement::Calculate_MoveTarget_Location();
		if (IsSucceed_Calculation == false) {
			//Exception
			m_FollowingPath.Empty();
			return;
		}

		/*m_MoveType_Before = m_MoveType_Current;
		int32 Num_RemainPath = m_FollowingPath.Num();
		if (Num_RemainPath > 0) {
			m_MoveType_Current = m_FollowingPath[0].eMoveType;
		}
		else {
			m_MoveType_Current = EHHM_MoveType::MT_OnGround;
		}*/

		return;
	}



	//if ( Currently wating for jump ) do FollowPath_Jump(DeltaTime);
	/*if (m_MoveType_Current == EHHM_MoveType::MT_Jump || m_MoveType_Current == EHHM_MoveType::MT_Fall
		|| m_MoveType_Current == EHHM_MoveType::MT_HorizontalJump_Left || m_MoveType_Current == EHHM_MoveType::MT_HorizontalJump_Right) {
		UHHM_Component_Movement::FollowPath_Jump(DeltaTime);
		//Do Switch
	}*/
	if (m_MoveType_Current != EHHM_MoveType::MT_OnGround) {
		switch (m_MoveType_Current)
		{
		case EHHM_MoveType::MT_Jump:
			UHHM_Component_Movement::FollowPath_Jump(DeltaTime);
			return;
			break;

		case EHHM_MoveType::MT_Fall:
			UHHM_Component_Movement::FollowPath_Fall(DeltaTime);
			return;
			break;

		case EHHM_MoveType::MT_HorizontalJump_Left:
		case EHHM_MoveType::MT_HorizontalJump_Right:
			UHHM_Component_Movement::FollowPath_HorizontalJump(DeltaTime);
			return;
			break;

		default:
			//Exception : not implemented input movetype
			return;
			break;
		}
	}



	//Following Path
	UHHM_Component_Movement::FollowPath_Walk(DeltaTime);
}



#pragma region Implement_FollowingPath
void UHHM_Component_Movement::FollowPath_Jump(float DeltaTime) {
	//if current jump is free-jump, call free-jump function instead.
	int32 JumpLength_Current = m_FollowingPath[0].MoveValue - 1;	//Because MoveValue is set as 1 more value than actual jump length

	if (JumpLength_Current >= m_MovementData.Jump_Vertical_FreeJump_Minimum) {
		UHHM_Component_Movement::FollowPath_Jump_Free(DeltaTime);
		return;
	}



	//if jump just started, reset MoveTimer
	if (m_MoveType_Before != EHHM_MoveType::MT_Jump) {
		float Duration_Animation = m_MovementData.Jump_Vertical_AnimationDuration[JumpLength_Current];
		m_Move_Timer = Duration_Animation;
	}

	//Decrease MoveTimer and move actor if movetimer reach zero on this tick.
	float MoveTimer_AfterTick = m_Move_Timer - DeltaTime;
	if (MoveTimer_AfterTick <= 0.0f) {
		FVector Location_Current = GetActorLocation();
		FVector Vec_CurrentToTarget = m_MoveTarget_Current - Location_Current;
		FQuat		Rotator = FQuat();
		FHitResult	HitResult = FHitResult();

		SafeMoveUpdatedComponent(Vec_CurrentToTarget, Rotator, false, HitResult);

		m_Move_Timer = 0.0f;
		return;
	}

	m_Move_Timer = MoveTimer_AfterTick;
}

void UHHM_Component_Movement::FollowPath_Jump_Free(float DeltaTime) {
	//Set target location as just directly above character
	//Current Location + Jumplength * tilesize
	//Start - End - During



	//if Jump just started reset MoveTimer and MoveState, and also fix move target location
	if (m_MoveType_Before != EHHM_MoveType::MT_Jump) {
		//Reset MoveTimer
		float Duration_Animation_Begin = m_MovementData.Jump_Vertical_FreeJump_Duration_Start;
		m_Move_Timer = Duration_Animation_Begin;

		//Reset MoveState
		m_MoveState_Current = EHHM_Entity_Movement_State::MoveState_Begin;



		//Fix MoveTarget Location
		UWorld* pWorld = nullptr;
		pWorld = GetWorld();
		if (pWorld == nullptr) {
			//Exception
			Abort_Path();
			return;
		}

		AGameModeBase* pGameMode_Raw = nullptr;
		pGameMode_Raw = pWorld->GetAuthGameMode();
		if (pGameMode_Raw == nullptr) {
			//Exception
			Abort_Path();
			return;
		}

		AHHM_GameMode_LocalMap* pGameMode = nullptr;
		pGameMode = Cast<AHHM_GameMode_LocalMap>(pGameMode_Raw);
		if (pGameMode == nullptr) {
			//Exception
			Abort_Path();
			return;
		}

		// HHM Note : if Optimizing work needed, these location calculating part might needs to be modified. calculate on this function instead calling some utility functions
		int32		Index_Current = m_FollowingPath[0].Index_Start;
		FVector2D	IndexLocation_Current = FVector2D();
		bool IsSucceed_SeperateIndex = pGameMode->Utility_Index_Seperate(IndexLocation_Current, Index_Current);
		if (IsSucceed_SeperateIndex == false) {
			//Exception
			Abort_Path();
			return;
		}
		
		int32 JumpLength = m_FollowingPath[0].MoveValue - 1;
		int32 MoveTarget_Height = JumpLength - 1;
		int32 Index_MoveTarget_Height = IndexLocation_Current.Y + MoveTarget_Height;
		FVector2D IndexLocation_MoveTarget = FVector2D(IndexLocation_Current.X, Index_MoveTarget_Height);

		FVector Location_MoveTarget = FVector();
		bool IsSucceed_CalculateLocation = pGameMode->Utility_Calculate_Location(Location_MoveTarget, IndexLocation_MoveTarget, 0.0f);
		if (IsSucceed_CalculateLocation == false) {
			//Exception
			Abort_Path();
			return;
		}

		m_MoveTarget_Current = Location_MoveTarget;
	}

	float MoveTimer_AfterTick = m_Move_Timer - DeltaTime;



	//Begin
	if (m_MoveState_Current == EHHM_Entity_Movement_State::MoveState_Begin) {
		if (MoveTimer_AfterTick <= 0.0f) {
			m_MoveState_Current = EHHM_Entity_Movement_State::MoveState_During;
			m_Move_Timer = 0.0f;
			return;
		}
	}



	//End
	if (m_MoveState_Current == EHHM_Entity_Movement_State::MoveState_End) {
		if (MoveTimer_AfterTick <= 0.0f) {
			m_MoveState_Current = EHHM_Entity_Movement_State::MoveState_Dummy;

			
			FVector		Location_Current = GetActorLocation();
			FVector		Vec_CurrentToTarget = m_MoveTarget_Current - Location_Current;

			AActor* pActor_Owner = GetOwner();
			if (pActor_Owner == nullptr) {
				//Exception
				Abort_Path();
				return;
			}
			FRotator	Rotator_Current = pActor_Owner->GetActorRotation();
			FQuat		Quat_Current = Rotator_Current.Quaternion();
			FHitResult	HitResult = FHitResult();
			bool IsSucceed_Move = SafeMoveUpdatedComponent(Vec_CurrentToTarget, Quat_Current, false, HitResult);
			if (IsSucceed_Move == false) {
				//Exception
				Abort_Path();
				return;
			}

			m_Move_Timer = 0.0f;
			
			return;
		}
	}



	//During
	if (m_MoveState_Current == EHHM_Entity_Movement_State::MoveState_During) {
		FVector Location_Current = GetActorLocation();
		float Location_Z_AfterTick = Location_Current.Z + (m_MovementData.Jump_Vertical_FreeJump_Speed * DeltaTime * m_TileSize);
		float Location_Z_MoveTarget = m_MoveTarget_Current.Z;

		FVector	Location_AfterTick = FVector(Location_Current.X, Location_Current.Y, Location_Z_AfterTick);
		
		if (Location_Z_AfterTick >= Location_Z_MoveTarget) {
			FVector		Vec_CurrentToTarget = m_MoveTarget_Current - Location_Current;

			AActor* pActor_Owner = GetOwner();
			if (pActor_Owner == nullptr) {
				//Exception
				Abort_Path();
				return;
			}
			FRotator	Rotator_Current = pActor_Owner->GetActorRotation();
			FQuat		Quat_Current = Rotator_Current.Quaternion();

			FHitResult	HitResult = FHitResult();
			bool		IsSucceed_Move = SafeMoveUpdatedComponent(Vec_CurrentToTarget, Quat_Current, false, HitResult);
			if (IsSucceed_Move == false) {
				//Exception
				Abort_Path();
				return;
			}

			m_MoveState_Current = EHHM_Entity_Movement_State::MoveState_End;
			
			m_Move_Timer = m_MovementData.Jump_Vertical_FreeJump_Duration_End;

			bool IsSucceed_CalculateMoveTarget = UHHM_Component_Movement::Calculate_MoveTarget_Location();
			if (IsSucceed_CalculateMoveTarget == false) {
				//Exception
				Abort_Path();
				return;
			}

			return;
		}
		else {
			FVector		Vec_CurrentToAfterTick = Location_AfterTick - Location_Current;

			AActor* pActor_Owner = GetOwner();
			if (pActor_Owner == nullptr) {
				//Exception
				Abort_Path();
				return;
			}
			FRotator	Rotator_Current = pActor_Owner->GetActorRotation();
			FQuat		Quat_Current = Rotator_Current.Quaternion();

			FHitResult	HitResult = FHitResult();
			bool		IsSucceed_Move = SafeMoveUpdatedComponent(Vec_CurrentToAfterTick, Quat_Current, false, HitResult);
			if (IsSucceed_Move == false) {
				//Exception
				Abort_Path();
				return;
			}
		}
	}

	//Exception
	Abort_Path();
	return;
}



void UHHM_Component_Movement::FollowPath_Fall(float DeltaTime) {
	int32 FallLength_Current = m_FollowingPath[0].MoveValue - 1;	//Because MoveValue is set as 1 more value than actaul jump length

	if (FallLength_Current >= m_MovementData.Fall_FreeFall_Minimum) {	//if FallLength is same or higher than freefall minimum length, do freefall
		UHHM_Component_Movement::FollowPath_Fall_Free(DeltaTime);
		return;
	}



	//if Fall just started, reset MoveTimer
	if (m_MoveType_Before != EHHM_MoveType::MT_Fall) {
		float Duration_Animation = m_MovementData.Fall_AnimationDuration[FallLength_Current];
		m_Move_Timer = Duration_Animation;
	}

	//Decrease MoveTimer and move actor if MoveTimer reach zero on this tick.
	float MoveTimer_AfterTick = m_Move_Timer - DeltaTime;
	if (MoveTimer_AfterTick <= 0.0f) {
		FVector Location_Current = GetActorLocation();
		FVector	Vec_CurrentToTarget = m_MoveTarget_Current - Location_Current;
		FQuat		Rotator = FQuat();
		FHitResult	HitResult = FHitResult();

		SafeMoveUpdatedComponent(Vec_CurrentToTarget, Rotator, false, HitResult);

		m_Move_Timer = 0.0f;
		return;
	}

	m_Move_Timer = MoveTimer_AfterTick;
}

void UHHM_Component_Movement::FollowPath_Fall_Free(float DeltaTime) {
	//Set target location as just directly below character
	//Current location + Jumplength + tilesize
	//Start - End - During



	//if Fall just started reset MoveTimer and MoveState, and also fix move target location
	if (m_MoveType_Before != EHHM_MoveType::MT_Fall) {
		//Reset MoveTimer
		float Duration_Animation_Begin = m_MovementData.Fall_FreeFall_Duration_Start;
		m_Move_Timer = Duration_Animation_Begin;

		//Reset MoveState
		m_MoveState_Current = EHHM_Entity_Movement_State::MoveState_Begin;



		//Fix MoveTarget Location
		UWorld* pWorld = nullptr;
		pWorld = GetWorld();
		if (pWorld == nullptr) {
			//Exception
			Abort_Path();
			return;
		}

		AGameModeBase* pGameMode_Raw = nullptr;
		pGameMode_Raw = pWorld->GetAuthGameMode();
		if (pGameMode_Raw == nullptr) {
			//Exception
			Abort_Path();
			return;
		}

		AHHM_GameMode_LocalMap* pGameMode = nullptr;
		pGameMode = Cast<AHHM_GameMode_LocalMap>(pGameMode_Raw);
		if (pGameMode == nullptr) {
			//Exception
			Abort_Path();
			return;
		}

		// HHM Note : if Optimizing work needed, these location calculating part might needs to be modified. calculate on this function instead calling some utility functions
		int32		Index_Current = m_FollowingPath[0].Index_Start;
		FVector2D	IndexLocation_Current = FVector2D();
		bool IsSucceed_SeperateIndex = pGameMode->Utility_Index_Seperate(IndexLocation_Current, Index_Current);
		if (IsSucceed_SeperateIndex == false) {
			//Exception
			Abort_Path();
			return;
		}

		int32 JumpLength = m_FollowingPath[0].MoveValue - 1;
		int32 MoveTarget_Height = JumpLength - 1;
		int32 Index_MoveTarget_Height = IndexLocation_Current.Y + MoveTarget_Height;
		FVector2D IndexLocation_MoveTarget = FVector2D(IndexLocation_Current.X, Index_MoveTarget_Height);

		FVector Location_MoveTarget = FVector();
		bool IsSucceed_CalculateLocation = pGameMode->Utility_Calculate_Location(Location_MoveTarget, IndexLocation_MoveTarget, 0.0f);
		if (IsSucceed_CalculateLocation == false) {
			//Exception
			Abort_Path();
			return;
		}

		m_MoveTarget_Current = Location_MoveTarget;
	}

	float MoveTimer_AfterTick = m_Move_Timer - DeltaTime;
}



void UHHM_Component_Movement::FollowPath_HorizontalJump(float DeltaTime) {

}

void UHHM_Component_Movement::FollowPath_HorizontalJump_Free(float DeltaTime) {

}



void UHHM_Component_Movement::FollowPath_Walk(float DeltaTime) {
	FVector Location_Current = GetActorLocation();

	//Get Target Direction
	FVector Direction_Current_To_Target = m_MoveTarget_Current - Location_Current;
	Direction_Current_To_Target.Y = 0.0f;
	Direction_Current_To_Target.Z = 0.0f;

	FVector Vec_Current_To_Target = Direction_Current_To_Target;
	Direction_Current_To_Target.Normalize();

	//Get Location of this entity should be after this tick
	FVector Location_AfterTick = Location_Current + (Direction_Current_To_Target * (m_Speed_Current * m_TileSize) * DeltaTime);

	//Get Distance of AfterTick and Target
	FVector Vec_Current_To_AfterTick = Location_AfterTick - Location_Current;
	Vec_Current_To_AfterTick.Y = 0.0f;
	Vec_Current_To_AfterTick.Z = 0.0f;

	float Distance_Current_To_AfterTick = Vec_Current_To_AfterTick.Size();
	float Distance_Current_To_Target = Vec_Current_To_Target.Size();

	//Compare The Distance and if Entity will be pass the target after this tick, set actor's location as target
	AActor* pActor_Owner = GetOwner();
	if (pActor_Owner == nullptr) {
		//Exception
		Abort_Path();
		return;
	}
	FRotator Rotator_Current = pActor_Owner->GetActorRotation();
	FQuat Quat_Current = Rotator_Current.Quaternion();
	FHitResult HitResult = FHitResult();
	if (Distance_Current_To_AfterTick > Distance_Current_To_Target) {
		SafeMoveUpdatedComponent(Vec_Current_To_Target, Quat_Current, false, HitResult);
	}
	else {
		SafeMoveUpdatedComponent(Vec_Current_To_AfterTick, Quat_Current, false, HitResult);
	}
}
#pragma endregion

bool UHHM_Component_Movement::Calculate_MoveTarget_Location(void) {
	int32 Num_FollowingPath = m_FollowingPath.Num();
	if (Num_FollowingPath <= 0) {
		//Exception
		return false;
	}

	UWorld* pWorld = nullptr;
	pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception
		return false;
	}

	AGameModeBase* pGameMode_Raw = nullptr;
	pGameMode_Raw = pWorld->GetAuthGameMode();
	if (pGameMode_Raw == nullptr) {
		//Exception
		return false;
	}

	AHHM_GameMode_LocalMap* pGameMode = nullptr;
	pGameMode = Cast<AHHM_GameMode_LocalMap>(pGameMode_Raw);
	if (pGameMode == nullptr) {
		//Exception
		return false;
	}

	FVector TargetLocation;
	bool IsCalculationSucceed = pGameMode->Utility_Calculate_Location(TargetLocation, m_FollowingPath[0].IndexLocation_Target, 0.0f);
	
	m_MoveTarget_Current = TargetLocation;

	return IsCalculationSucceed;
	
}

void UHHM_Component_Movement::Abort_Path(void) {
	m_FollowingPath.Empty();
	m_Move_Timer = 0.0f;
	m_MoveTarget_Current = FVector();
}