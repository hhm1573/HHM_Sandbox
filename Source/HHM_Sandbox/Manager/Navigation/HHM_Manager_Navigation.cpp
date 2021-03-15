// Fill out your copyright notice in the Description page of Project Settings.

#include "HHM_Manager_Navigation.h"

#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Base/GameMode/HHM_GameMode_LocalMap.h"

#include "Header/Struct.h"
#include "Data/LocalMap/LocalMap.h"

#include "Manager/Navigation/NavLink/HHM_NavLinkProxy.h"
#include "Manager/Navigation/NavLink/HHM_NavLinkProxy_H1.h"
#include "Manager/Navigation/NavLink/HHM_NavLinkProxy_H2.h"

#include "Manager/Math/Grid/HHM_Manager_Math_Grid.h"

#include "PathFinder/HHM_PathFinder.h"



AHHM_Manager_Navigation::AHHM_Manager_Navigation(void) {

}

void AHHM_Manager_Navigation::Tick(float DeltaTime) {

}

void AHHM_Manager_Navigation::BeginPlay(void) {
	Super::BeginPlay();
	//Refresh_Region();
}



TArray<FHHM_PathNodeData> AHHM_Manager_Navigation::Search_Path(const ALocalMap* _pLocalMap, FVector2D _location_Start, FVector2D _location_End, const FHHM_Parameter_PathFind& _pathfind_Params) const
{
	return UHHM_PathFinder::Search_Path(_pLocalMap, _location_Start, _location_End, _pathfind_Params);
}
