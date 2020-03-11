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
	m_FollowingPath.Empty();
}

void UHHM_Component_Movement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) {
		return;
	}
	


	if (m_FollowingPath.Num() > 0) {
		FollowPath();
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



void UHHM_Component_Movement::FollowPath(void) {
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

	}
	//Get EndVector


	const ALocalMap* LocalMap = nullptr;
	LocalMap = pGameMode->Get_LocalMap_Const();

	FVector Location_Current = GetActorLocation();



	EHHM_MoveType MoveType_Current = m_FollowingPath[0].eMoveType;
}
