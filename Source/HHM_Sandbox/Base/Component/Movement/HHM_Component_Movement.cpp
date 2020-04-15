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
}

void UHHM_Component_Movement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) {
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
	int32 Num_MultiplicationElements = m_Container_Speed_MultiplicationElement.Num();

	if (Num_MultiplicationElements > 0) {
		TArray<FHHM_Movement_SpeedMultiplicationData> Container_Speed_MultiplicationElement_Sorted;
		Container_Speed_MultiplicationElement_Sorted = m_Container_Speed_MultiplicationElement;

		// Note :: here sorting job is done by simple bubble sorting. change it to better sorting algorithm if any performance issue occurs
	}



	m_Speed_Current = Speed_Current_Tick;
}



void UHHM_Component_Movement::FollowPath(float DeltaTime) {

	//if ( Currently wating for jump ) do FollowPath_Jump(DeltaTime);
	if (m_MoveType_Current == EHHM_MoveType::MT_Jump || m_MoveType_Current == EHHM_MoveType::MT_Fall
		|| m_MoveType_Current == EHHM_MoveType::MT_HorizontalJump_Left || m_MoveType_Current == EHHM_MoveType::MT_HorizontalJump_Right) {
		UHHM_Component_Movement::FollowPath_Jump(DeltaTime);
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

	float TileSize = 100.0f;



	int32		Index_Target = m_FollowingPath[0].Index_Target;
	FVector2D	IndexLocation_Target = FVector2D();
	if (pGameMode->Utility_Index_Seperate(IndexLocation_Target, Index_Target) == false) {
		//Exception
		return;
	}
	FVector		Location_Target = FVector();
	if (pGameMode->Utility_Calculate_Location(Location_Target, IndexLocation_Target) == false) {
		//Exception
		return;
	}


	/*const ALocalMap* LocalMap = nullptr;
	LocalMap = pGameMode->Get_LocalMap_Const();*/

	FVector Location_Current = GetActorLocation();
	float	Location_Start_X = Location_Current.X - ((m_EntitySize_Horizontal - 1) * (TileSize * 0.5f));
	float	Location_Start_Y = Location_Current.Y - ((m_EntitySize_Vertical - 1) * (TileSize * 0.5f));
	FVector Location_Start = FVector(Location_Start_X, 0.0f, Location_Start_Y);
	FVector2D IndexLocation_Start = FVector2D(Location_Start.X / TileSize, Location_Start.Z / TileSize);



	EHHM_MoveType MoveType_Current = m_FollowingPath[0].eMoveType;



	//Check is entity at target point
	FVector Vec_Distance_Target = Location_Target - Location_Start;
	float	Distance_Target = Vec_Distance_Target.Size();
	//Change this Value later ---- ---- ---- ----
	float	Acceptable_Distance = 10.0f;
	if (Distance_Target <= Acceptable_Distance) {
		m_FollowingPath.RemoveAt(0);
		return;
	}



	//Following Path
	if (MoveType_Current == EHHM_MoveType::MT_OnGround) {
		//Get Target Direction
		FVector Direction_Current_To_Target = Location_Target - Location_Current;
		Direction_Current_To_Target.Y = 0.0f;
		Direction_Current_To_Target.Z = 0.0f;
		Direction_Current_To_Target.Normalize();

		//Get Location of this entity should be after this tick
		FVector Location_AfterTick = Location_Current + (Direction_Current_To_Target * (m_Speed_Current * TileSize) * DeltaTime);

		//Get Distance of AfterTick and Target
		FVector Direction_Current_To_AfterTick = Location_AfterTick - Location_Current;
		Direction_Current_To_AfterTick.Y = 0.0f;
		Direction_Current_To_AfterTick.Z = 0.0f;

		float Distance_Current_To_AfterTick = Direction_Current_To_AfterTick.Size();
		float Distance_Current_To_Target = Direction_Current_To_Target.Size();

		//Compare The Distance and if Entity will be pass the target after this tick, set actor's location as target
		
	}
	else if (MoveType_Current == EHHM_MoveType::MT_Jump) {

	}
}



void UHHM_Component_Movement::FollowPath_Jump(float DeltaTime) {

}

void UHHM_Component_Movement::FollowPath_Walk(float DeltaTime) {

}