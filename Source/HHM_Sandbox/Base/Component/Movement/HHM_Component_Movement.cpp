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

	TArray<FHHM_PathNodeData> Path_Find = pManager_Navigation->Search_Path(pLocalMap, Vec_Location_Start, Vec_Location_End, m_EntitySize_Horizontal, m_EntitySize_Vertical, m_MaxJumpHeight, m_MaxFallHeight, m_MaxHorizontalJumpLength);
	//pManager_Navigation->Search_Path(pLocalMap, )
	m_FollowingPath = Path_Find;

	

	return true;
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

	//if ( Currently wating for jump ) do FollowPath_Jump(DeltaTime);
	if (m_MoveType_Current == EHHM_MoveType::MT_Jump || m_MoveType_Current == EHHM_MoveType::MT_Fall
		|| m_MoveType_Current == EHHM_MoveType::MT_HorizontalJump_Left || m_MoveType_Current == EHHM_MoveType::MT_HorizontalJump_Right) {
		UHHM_Component_Movement::FollowPath_Jump(DeltaTime);
		//Do Switch
	}



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



	int32		Index_Target = m_FollowingPath[0].Index_Target;
	FVector2D	IndexLocation_Target = FVector2D();
	if (pGameMode->Utility_Index_Seperate(IndexLocation_Target, Index_Target) == false) {
		//Exception
		return;
	}
	FVector		Location_Target = FVector();
	if (pGameMode->Utility_Calculate_Location(Location_Target, IndexLocation_Target) == false) {		//HHM Note : Y Level Control
		//Exception
		return;
	}


	/*const ALocalMap* LocalMap = nullptr;
	LocalMap = pGameMode->Get_LocalMap_Const();*/

	FVector Location_Current = GetActorLocation();
	float	Location_Start_X = Location_Current.X - ((m_EntitySize_Horizontal - 1) * (m_TileSize * 0.5f));
	float	Location_Start_Y = Location_Current.Y - ((m_EntitySize_Vertical - 1) * (m_TileSize * 0.5f));
	FVector Location_Start = FVector(Location_Start_X, 0.0f, Location_Start_Y);														//HHM Note : Y Level Control
	FVector2D IndexLocation_Start = FVector2D(Location_Start.X / m_TileSize, Location_Start.Z / m_TileSize);



	EHHM_MoveType MoveType_Current = m_FollowingPath[0].eMoveType;



	//Check is entity at target point
	FVector Vec_Distance_Target = Location_Target - Location_Start;
	float	Distance_Target = Vec_Distance_Target.Size();
	//HHM Note : Change this Value later ---- ---- ---- ----
	float	Acceptable_Distance = 10.0f;

	//Return if entity is already at target point
	if (Distance_Target <= Acceptable_Distance) {
		m_FollowingPath.RemoveAt(0);

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



	//Following Path
	UHHM_Component_Movement::FollowPath_Walk(DeltaTime, Location_Current, Location_Target);
}



void UHHM_Component_Movement::FollowPath_Jump(float DeltaTime) {

}

void UHHM_Component_Movement::FollowPath_Walk(float DeltaTime, const FVector& _location_Current, const FVector& _location_Target) {
	//Get Target Direction
	FVector Direction_Current_To_Target = _location_Target - _location_Current;
	Direction_Current_To_Target.Y = 0.0f;
	Direction_Current_To_Target.Z = 0.0f;

	FVector Vec_Current_To_Target = Direction_Current_To_Target;
	Direction_Current_To_Target.Normalize();

	//Get Location of this entity should be after this tick
	FVector Location_AfterTick = _location_Current + (Direction_Current_To_Target * (m_Speed_Current * m_TileSize) * DeltaTime);

	//Get Distance of AfterTick and Target
	FVector Vec_Current_To_AfterTick = Location_AfterTick - _location_Current;
	Vec_Current_To_AfterTick.Y = 0.0f;
	Vec_Current_To_AfterTick.Z = 0.0f;

	float Distance_Current_To_AfterTick = Vec_Current_To_AfterTick.Size();
	float Distance_Current_To_Target = Vec_Current_To_Target.Size();

	//Compare The Distance and if Entity will be pass the target after this tick, set actor's location as target
	FQuat Rotator = FQuat();
	FHitResult HitResult = FHitResult();
	if (Distance_Current_To_AfterTick > Distance_Current_To_Target) {
		SafeMoveUpdatedComponent(Vec_Current_To_Target, Rotator, false, HitResult);
	}
	else {
		SafeMoveUpdatedComponent(Vec_Current_To_AfterTick, Rotator, false, HitResult);
	}
}