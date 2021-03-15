// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Component_Movement.h"
#include "Header/Struct_Pathfinder.h"

#include "Header/Macro.h"

#include "Entity/HHM_Entity.h"

#include "Engine/World.h"
#include "Base/GameMode/HHM_GameMode_LocalMap.h"
#include "Manager/Navigation/HHM_Manager_Navigation.h"
#include "Data/LocalMap/LocalMap.h"
#include "Header/Struct_Pathfinder.h"

#include "Manager/Math/Grid/HHM_Manager_Math_Grid.h"



UHHM_Component_Movement::UHHM_Component_Movement() {

}

void UHHM_Component_Movement::Temp_Set_MoveData(FHHM_Entity_MovementData& _moveData)
{
	m_MovementData = _moveData;
	return;
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



	//Check Is Falling
	if (m_bIsFalling == false) {
		bool IsFloor_Standalbe = Check_IsBelowFloor_Standable();
		m_bIsFalling = !(IsFloor_Standalbe);
	}

	
	if (m_bIsFalling == true) {												//If Falling, Do Some Falling job
		//If It just start to falling abort path
		if (m_bIsFalling_Before == false) {
			Abort_Path();
		}

		Update_Falling(DeltaTime);

		//Update IsFalling_Before
		m_bIsFalling_Before = m_bIsFalling;

		return;
	}
	else if (m_bIsRecovering == true) {										//If Falling ended, do recover
		Update_Recovering(DeltaTime);										//Recovering will be ended by state machine, after playing recovering animations.
		//HHM Note: Remove this part
		m_bIsRecovering = false;
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
	// HHM Note : Save Manager's pointer if that kind of thing needed

	//Get Navigation Manager
	UWorld* pWorld = GetWorld();
	if (pWorld == nullptr) {
		//Exception
		return false;
	}

	AGameModeBase* pGameMode_Raw = pWorld->GetAuthGameMode();
	if (pGameMode_Raw == nullptr) {
		//Exception
		return false;
	}

	AHHM_GameMode_LocalMap* pGameMode = Cast <AHHM_GameMode_LocalMap>(pGameMode_Raw);
	if (pGameMode == nullptr) {
		//Exception
		return false;
	}

	AHHM_Manager_Navigation* pManager_Navigation = pGameMode->Get_Manager_Navigation();
	if (pManager_Navigation == nullptr) {
		//Exception
		return false;
	}



	//Get LocalMap & MapInfo
	AActor* pOwner_Raw = GetOwner();
	if (pOwner_Raw == nullptr) {
		//Exception
		return false;
	}

	AHHM_Entity* pOwner = Cast<AHHM_Entity>(pOwner_Raw);
	if (pOwner == nullptr) {
		//Exception
		return false;
	}

	const ALocalMap* pLocalMap = pOwner->Get_LocalMap_Const();
	if (pLocalMap == nullptr) {
		//Exception
		return false;
	}

	const FHHM_MapInfo& MapInfo = pLocalMap->Get_MapInfo_ConstRef();



	FVector Vec_Location_Current = GetActorLocation();
	FVector2D Vec_Location_Start = FVector2D();
	bool IsSucceed_Calculate_IndexLocation = AHHM_Manager_Math_Grid::Convert_Translation_To_IndexLocation(Vec_Location_Start, Vec_Location_Current, MapInfo);
	if (IsSucceed_Calculate_IndexLocation == false) {
		return false;
	}
	FVector2D Vec_Location_End = FVector2D(_index_Horizontal, _index_Vertical);



	//Build Pathfind params
	FHHM_Parameter_PathFind Pathfind_Params = FHHM_Parameter_PathFind(m_EntitySize_Horizontal, m_EntitySize_Vertical, m_MovementData.Jump_Vertical_MaxHeight, m_MovementData.DownJump_MaxHeight, m_MovementData.Jump_Horizontal_MaxLength);

	TArray<FHHM_PathNodeData> Path_Find = pManager_Navigation->Search_Path(pLocalMap, Vec_Location_Start, Vec_Location_End, Pathfind_Params);
	//pManager_Navigation->Search_Path(pLocalMap, )
	m_FollowingPath = Path_Find;

	bool IsSucceed_Calculation = UHHM_Component_Movement::Calculate_MoveTarget_Location();
	if (IsSucceed_Calculation == false) {
		m_FollowingPath.Empty();
		return false;
	}

	

	return true;
}

bool UHHM_Component_Movement::Change_Face_Direction(bool _toLeft)
{
	if (Super::UpdatedComponent == nullptr) {
		return false;
	}

	FVector Scale = Super::UpdatedComponent->GetComponentScale();
	float Scale_X = Scale.X;

	if (_toLeft == true) {
		if (Scale_X < 0.0f) {
			FVector Scale_After = Scale;
			Scale_After.X = Scale_X * -1.0f;
			Super::UpdatedComponent->SetWorldScale3D(Scale_After);
		}
	}
	else {
		if (Scale_X > 0.0f) {
			FVector Scale_After = Scale;
			Scale_After.X = Scale_X * -1.0f;
			Super::UpdatedComponent->SetWorldScale3D(Scale_After);
		}
	}

	return true;
}



#pragma region Getter



int32 UHHM_Component_Movement::Get_MoveValue()
{
	int32 Num_Path_Remaining = m_FollowingPath.Num();
	return Num_Path_Remaining == 0 ? -1 : m_FollowingPath[0].MoveValue;
}

FVector UHHM_Component_Movement::Get_Location()
{
	if (Super::UpdatedComponent == nullptr) {
		//Exception
		return FVector::ZeroVector;
	}
	
	FVector Vec_Location = Super::UpdatedComponent->GetComponentLocation();
	return Vec_Location;
}

FVector UHHM_Component_Movement::Get_Location_BottomLeft()
{
	//Below code is for getting BottomLeft corner location of bottomleft tile.
	/*FVector Vec_Location = Super::UpdatedComponent->GetComponentLocation();
	FVector Vec_Size_Half_Entity_HorizontalOnly = FVector(m_EntitySize_Horizontal * HHM_TILE_SIZE * 0.5f, 0.0f, 0.0f);
	FVector Vec_Location_BottomLeft = Vec_Location - Vec_Size_Half_Entity_HorizontalOnly;

	return Vec_Location_BottomLeft;*/



	//Get Bottom-Center location of Bottom-Left tile
	if (Super::UpdatedComponent == nullptr) {
		//Exception
		return FVector::ZeroVector;
	}

	FVector Vec_Location = Super::UpdatedComponent->GetComponentLocation();
	float Distance_To_BottomLeft_EntityTile = (m_EntitySize_Horizontal - 1) * (HHM_TILE_SIZE * 0.5f);
	FVector Vec_Location_BottomLeft = FVector(Vec_Location.X - Distance_To_BottomLeft_EntityTile, Vec_Location.Y, Vec_Location.Z);

	return Vec_Location_BottomLeft;
}

FRotator UHHM_Component_Movement::Get_Rotation_Safe()
{
	FRotator Rotator_Return = FRotator(0.0f);

	if (Super::UpdatedComponent != nullptr) {
		Rotator_Return = Super::UpdatedComponent->GetComponentRotation();
	}

	return Rotator_Return;
}



#pragma endregion



int32	UHHM_Component_Movement::Add_Speed_ModificationElement(float _modifyValue) {
	return 0;
}

int32	UHHM_Component_Movement::Add_Speed_MultiplicationElement(int32 _weight, float _multiplyValue) {
	return 0;
}

#pragma region State_Check

bool UHHM_Component_Movement::Is_OnAir_Begin(void) {
	bool IsCurrentlyOnAir = m_MoveType_Current == EHHM_MoveType::MT_Jump || m_MoveType_Current == EHHM_MoveType::MT_DownJump ||
		m_MoveType_Current == EHHM_MoveType::MT_HorizontalJump_Left || m_MoveType_Current == EHHM_MoveType::MT_HorizontalJump_Right ? true : false;

	return IsCurrentlyOnAir == true && m_MoveType_Before == EHHM_MoveType::MT_OnGround ? true : false;
}

bool UHHM_Component_Movement::Is_OnAir(void) {
	bool IsCurrentlyOnAir = m_MoveType_Current == EHHM_MoveType::MT_Jump || m_MoveType_Current == EHHM_MoveType::MT_DownJump ||
		m_MoveType_Current == EHHM_MoveType::MT_HorizontalJump_Left || m_MoveType_Current == EHHM_MoveType::MT_HorizontalJump_Right ? true : false;

	bool IsOnAirBefore = m_MoveType_Before == EHHM_MoveType::MT_Jump || m_MoveType_Before == EHHM_MoveType::MT_DownJump ||
		m_MoveType_Before == EHHM_MoveType::MT_HorizontalJump_Left || m_MoveType_Before == EHHM_MoveType::MT_HorizontalJump_Right ? true : false;

	return IsCurrentlyOnAir == true && IsOnAirBefore == true ? true : false;
}

bool UHHM_Component_Movement::Is_OnAir_End(void) {
	bool IsOnAirBefore = m_MoveType_Before == EHHM_MoveType::MT_Jump || m_MoveType_Before == EHHM_MoveType::MT_DownJump ||
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



void UHHM_Component_Movement::Update_Falling(float DeltaTime)
{
	//Get MapInfo
	AActor* pOwner_Raw = nullptr;
	pOwner_Raw = GetOwner();
	if (pOwner_Raw == nullptr) {
		//Exception
		return;
	}

	AHHM_Entity* pOwner = nullptr;
	pOwner = Cast<AHHM_Entity>(pOwner_Raw);
	if (pOwner == nullptr) {
		//Exception
		return;
	}

	const ALocalMap* pLocalMap = pOwner->Get_LocalMap_Const();
	if (pLocalMap == nullptr) {
		//Exception
		return;
	}

	const FHHM_MapInfo& MapInfo = pLocalMap->Get_MapInfo_ConstRef();



	//If recovering is true, set it false.
	if (m_bIsRecovering == true) {
		m_bIsRecovering = false;
	}

	//Get After Location for this tick
	FVector Vec_Location_Current = Get_Location();

	FVector Vec_Location_After_GravitationalFall = Vec_Location_Current;
	Vec_Location_After_GravitationalFall.Z = Vec_Location_After_GravitationalFall.Z - (HHM_GRAVITATIONAL_ACCELERATION * DeltaTime);



	//If Entity is currently located on standable area, check if it goes through tile. and if it goes through block the movement and start recovering proccess
	bool IsBelowFloor_Standable = Check_IsBelowFloor_Standable();
	if (IsBelowFloor_Standable == true) {
		FVector Vec_Location_BottomLeft = Get_Location_BottomLeft();

		FVector2D IndexLocation_BottomLeft = FVector2D(-1.0f, -1.0f);
		bool IsConvert_Succeed = AHHM_Manager_Math_Grid::Convert_Translation_To_IndexLocation(IndexLocation_BottomLeft, Vec_Location_BottomLeft, MapInfo);
		if (IsConvert_Succeed == false) {
			//Exception
			return;
		}

		FVector Vec_Location_Tile_BottomLeftCorner = FVector(-1.0f);
		bool IsSucceed_ReverseConvert = AHHM_Manager_Math_Grid::Convert_IndexLocation_To_Translation(Vec_Location_Tile_BottomLeftCorner, IndexLocation_BottomLeft, MapInfo);
		if (IsSucceed_ReverseConvert == false) {
			//Exception
			return;
		}

		float FloorHeight = Vec_Location_Tile_BottomLeftCorner.Z;

		if (Vec_Location_After_GravitationalFall.Z <= FloorHeight) {	//Falling End
			FVector Vec_Location_AfterTick = Vec_Location_After_GravitationalFall;
			Vec_Location_AfterTick.Z = FloorHeight;
			
			FVector Vec_Delta = Vec_Location_AfterTick - Vec_Location_Current;
			FRotator Rotator_Current = Super::UpdatedComponent->GetComponentRotation();
			FHitResult HitResult = FHitResult();
			bool IsSucceed_Move = SafeMoveUpdatedComponent(Vec_Delta, Rotator_Current, false, HitResult);
			if (IsSucceed_Move == false) {
				//Exception
				return;
			}

			m_bIsFalling = false;
			m_bIsRecovering = true;

			return;
		}
	}



	FVector Vec_Delta = Vec_Location_After_GravitationalFall - Vec_Location_Current;
	FRotator Rotator_Current = Super::UpdatedComponent->GetComponentRotation();
	FHitResult HitResult = FHitResult();
	bool IsSucceed_Move = SafeMoveUpdatedComponent(Vec_Delta, Rotator_Current, false, HitResult);
	if (IsSucceed_Move == false) {
		//Exception
		return;
	}


	
	//If below area is standable and after tick location is going through that area, set after-tick location on surface of that area and set falling false and recovery as true
	//If below area is not standable, move to after location
}

void UHHM_Component_Movement::Update_Recovering(float DeltaTime)
{
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
	Vec_CurrentToTarget.Y = 0.0f;
	float	Distance_Target = Vec_CurrentToTarget.Size();
	//HHM Note : Change this Value later ---- ---- ---- ----
	float	Acceptable_Distance = 0.1f;

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



	//Update Face Direction
	if (Vec_CurrentToTarget.X <= 0.0f) {
		Change_Face_Direction(true);
	}
	else {
		Change_Face_Direction(false);
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

		case EHHM_MoveType::MT_DownJump:
			UHHM_Component_Movement::FollowPath_DownJump(DeltaTime);
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



	//Determine whether moving direction is horizontal or vertical
	float Distance_Horizontal = FMath::Abs(Vec_CurrentToTarget.X);
	float Distance_Vertical = FMath::Abs(Vec_CurrentToTarget.Z);
	if (Distance_Horizontal < 0 || Distance_Vertical < 0) {
		//Exception
		// HHM Note : Remove this if FMath::Abs will always return absolute value
		return;
	}
	
	bool IsDirection_Vertical = Distance_Vertical > Distance_Horizontal ? true : false;



	//Following Path
	if (IsDirection_Vertical == false) {
		UHHM_Component_Movement::FollowPath_Walk(DeltaTime);
	}
	else {
		UHHM_Component_Movement::FollowPath_Ladder(DeltaTime);
	}
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
		float Duration_Animation = m_MovementData.Jump_Vertical_AnimationDuration[JumpLength_Current - 1];	// 1 block jump = 1 JumpLength_Current
		m_Move_Timer = Duration_Animation;
	}

	//Decrease MoveTimer and move actor if movetimer reach zero on this tick.
	float MoveTimer_AfterTick = m_Move_Timer - DeltaTime;
	if (MoveTimer_AfterTick <= 0.0f) {
		FVector Location_Current = GetActorLocation();
		FVector Vec_CurrentToTarget = m_MoveTarget_Current - Location_Current;
		FRotator	Rotator = Get_Rotation_Safe();
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
		//Get LocalMap & MapInfo
		AActor* pOwner_Raw = GetOwner();
		if (pOwner_Raw == nullptr) {
			//Exception
			Abort_Path();
			return;
		}

		AHHM_Entity* pOwner = Cast<AHHM_Entity>(pOwner_Raw);
		if (pOwner == nullptr) {
			//Exception
			Abort_Path();
			return;
		}

		const ALocalMap* pLocalMap = pOwner->Get_LocalMap_Const();
		if (pLocalMap == nullptr) {
			//Exception
			Abort_Path();
			return;
		}

		const FHHM_MapInfo& MapInfo = pLocalMap->Get_MapInfo_ConstRef();



		// HHM Note : if Optimizing work needed, these location calculating part might needs to be modified. calculate on this function instead calling some utility functions
		int32		Index_Current = m_FollowingPath[0].Index_Start;
		FVector2D	IndexLocation_Current = FVector2D();
		bool IsSucceed_SeperateIndex = AHHM_Manager_Math_Grid::Index_Seperate(IndexLocation_Current, Index_Current, MapInfo);
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
		bool IsSucceed_CalculateLocation = AHHM_Manager_Math_Grid::Convert_IndexLocation_To_Translation(Location_MoveTarget, IndexLocation_MoveTarget, MapInfo);
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
			FRotator	Rotator_Current = Get_Rotation_Safe();
			FHitResult	HitResult = FHitResult();
			bool IsSucceed_Move = SafeMoveUpdatedComponent(Vec_CurrentToTarget, Rotator_Current, false, HitResult);
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
			FRotator	Rotator_Current = Get_Rotation_Safe();

			FHitResult	HitResult = FHitResult();
			bool		IsSucceed_Move = SafeMoveUpdatedComponent(Vec_CurrentToTarget, Rotator_Current, false, HitResult);
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
			FRotator	Rotator_Current = Get_Rotation_Safe();

			FHitResult	HitResult = FHitResult();
			bool		IsSucceed_Move = SafeMoveUpdatedComponent(Vec_CurrentToAfterTick, Rotator_Current, false, HitResult);
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



void UHHM_Component_Movement::FollowPath_DownJump(float DeltaTime) {
	int32 JumpDownLength_Current = m_FollowingPath[0].MoveValue - 1;	//Because MoveValue is set as 1 more value than actaul jump length

	if (JumpDownLength_Current >= m_MovementData.DownJump_FreeFall_Minimum) {	//if JumpDownLength is same or higher than free JumpDown minimum length, do free JumpDown
		UHHM_Component_Movement::FollowPath_DownJump_Free(DeltaTime);
		return;
	}



	//if JumpDown just started, reset MoveTimer
	if (m_MoveType_Before != EHHM_MoveType::MT_DownJump) {
		float Duration_Animation = m_MovementData.DownJump_AnimationDuration[JumpDownLength_Current - 1];
		m_Move_Timer = Duration_Animation;
	}

	//Decrease MoveTimer and move actor if MoveTimer reach zero on this tick.
	float MoveTimer_AfterTick = m_Move_Timer - DeltaTime;
	if (MoveTimer_AfterTick <= 0.0f) {
		FVector Location_Current = GetActorLocation();
		FVector	Vec_CurrentToTarget = m_MoveTarget_Current - Location_Current;
		FRotator	Rotator = Get_Rotation_Safe();
		FHitResult	HitResult = FHitResult();

		SafeMoveUpdatedComponent(Vec_CurrentToTarget, Rotator, false, HitResult);

		m_Move_Timer = 0.0f;
		return;
	}

	m_Move_Timer = MoveTimer_AfterTick;
}

void UHHM_Component_Movement::FollowPath_DownJump_Free(float DeltaTime) {
	//Set target location as just directly below character
	//Current location + Jumplength + tilesize
	//Start - End - During



	//if JumpDown just started reset MoveTimer and MoveState, and also fix move target location
	if (m_MoveType_Before != EHHM_MoveType::MT_DownJump) {
		//Reset MoveTimer
		float Duration_Animation_Begin = m_MovementData.DownJump_FreeFall_Duration_Start;
		m_Move_Timer = Duration_Animation_Begin;

		//Reset MoveState
		m_MoveState_Current = EHHM_Entity_Movement_State::MoveState_Begin;



		//Fix MoveTarget Location
		//Get LocalMap & MapInfo
		AActor* pOwner_Raw = GetOwner();
		if (pOwner_Raw == nullptr) {
			//Exception
			Abort_Path();
			return;
		}

		AHHM_Entity* pOwner = Cast<AHHM_Entity>(pOwner_Raw);
		if (pOwner == nullptr) {
			//Exception
			Abort_Path();
			return;
		}

		const ALocalMap* pLocalMap = pOwner->Get_LocalMap_Const();
		if (pLocalMap == nullptr) {
			//Exception
			Abort_Path();
			return;
		}

		const FHHM_MapInfo& MapInfo = pLocalMap->Get_MapInfo_ConstRef();



		// HHM Note : if Optimizing work needed, these location calculating part might needs to be modified. calculate on this function instead calling some utility functions
		int32		Index_Current = m_FollowingPath[0].Index_Start;
		FVector2D	IndexLocation_Current = FVector2D();
		bool IsSucceed_SeperateIndex = AHHM_Manager_Math_Grid::Index_Seperate(IndexLocation_Current, Index_Current, MapInfo);
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
		bool IsSucceed_CalculateLocation = AHHM_Manager_Math_Grid::Convert_IndexLocation_To_Translation(Location_MoveTarget, IndexLocation_MoveTarget, MapInfo);
		if (IsSucceed_CalculateLocation == false) {
			//Exception
			Abort_Path();
			return;
		}

		m_MoveTarget_Current = Location_MoveTarget;
	}

	float MoveTimer_AfterTick = m_Move_Timer - DeltaTime;

	//HHM Note : Note Finished
}



void UHHM_Component_Movement::FollowPath_HorizontalJump(float DeltaTime) {
	// 작업하기 전에 위의 점프 부분의 로테이터 부분 수정. (노트 참조) 완료.
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
	FRotator Rotator_Current = Get_Rotation_Safe();
	FHitResult HitResult = FHitResult();
	if (Distance_Current_To_AfterTick > Distance_Current_To_Target) {
		SafeMoveUpdatedComponent(Vec_Current_To_Target, Rotator_Current, false, HitResult);
	}
	else {
		SafeMoveUpdatedComponent(Vec_Current_To_AfterTick, Rotator_Current, false, HitResult);
	}
}
void UHHM_Component_Movement::FollowPath_Ladder(float DeltaTile)
{
	return;
}
#pragma endregion



bool UHHM_Component_Movement::Calculate_MoveTarget_Location(void) {
	int32 Num_FollowingPath = m_FollowingPath.Num();
	if (Num_FollowingPath <= 0) {
		//Exception
		return false;
	}

	//Get LocalMap & MapInfo
	AActor* pOwner_Raw = GetOwner();
	if (pOwner_Raw == nullptr) {
		//Exception
		Abort_Path();
		return false;
	}

	AHHM_Entity* pOwner = Cast<AHHM_Entity>(pOwner_Raw);
	if (pOwner == nullptr) {
		//Exception
		Abort_Path();
		return false;
	}

	const ALocalMap* pLocalMap = pOwner->Get_LocalMap_Const();
	if (pLocalMap == nullptr) {
		//Exception
		Abort_Path();
		return false;
	}

	const FHHM_MapInfo& MapInfo = pLocalMap->Get_MapInfo_ConstRef();



	FVector TargetLocation;
	bool IsCalculationSucceed = AHHM_Manager_Math_Grid::Convert_IndexLocation_To_Translation(TargetLocation, m_FollowingPath[0].IndexLocation_Target, MapInfo);

	//Make it center of tile
	FVector Location_Offset_TileCenter = FVector(HHM_TILE_SIZE * 0.5f, 0.0f, 0.0f);
	
	m_MoveTarget_Current = TargetLocation + Location_Offset_TileCenter;

	return IsCalculationSucceed;
	
}

bool UHHM_Component_Movement::Check_IsBelowFloor_Standable(void)
{
	AActor* pOwner_Raw = nullptr;
	pOwner_Raw = GetOwner();
	if (pOwner_Raw == nullptr) {
		//Exception
		return false;
	}

	AHHM_Entity* pOwner = nullptr;
	pOwner = Cast<AHHM_Entity>(pOwner_Raw);
	if (pOwner == nullptr) {
		//Exception
		return false;
	}

	ALocalMap* pLocalMap = nullptr;
	pLocalMap = pOwner->Get_LocalMap();
	if (pLocalMap == nullptr) {
		//Exception
		return false;
	}


	
	//Get MapInfo
	const FHHM_MapInfo MapInfo = pLocalMap->Get_MapInfo_ConstRef();

	//Get LeftBottom Location
	FVector Vec_Location_BottomLeft = Get_Location_BottomLeft();

	//Get Index Of BottomLeft's location
	int32 Index_BottomLeft = -1;
	bool IsConvertSucceed = AHHM_Manager_Math_Grid::Convert_Translation_To_Index(Index_BottomLeft, Vec_Location_BottomLeft, MapInfo);
	if (IsConvertSucceed == false) {
		//Exception
		return false;
	}

	bool IsAreaStandable = pLocalMap->IsAreaStandable(Index_BottomLeft, m_EntitySize_Horizontal, m_EntitySize_Vertical);

	return IsAreaStandable;
}

void UHHM_Component_Movement::Abort_Path(void) {
	m_FollowingPath.Empty();
	m_Move_Timer = 0.0f;
	m_MoveTarget_Current = FVector();
}