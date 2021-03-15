// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/HHM_Manager.h"

#include "Header/Struct.h"
#include "Header/Struct_Tile.h"
#include "Header/Struct_Navigation.h"
#include "Header/Struct_Pathfinder.h"
#include "PathFinder/HHM_Parameter_PathFind.h"

#include "Data/LocalMap/LocalMap.h"

#include "HHM_Manager_Navigation.generated.h"

class AHHM_NavLinkProxy;

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API AHHM_Manager_Navigation : public AHHM_Manager
{
	GENERATED_BODY()

public:
	AHHM_Manager_Navigation();

public:
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;



public:
	UFUNCTION(BlueprintCallable)
		TArray<FHHM_PathNodeData> Search_Path(const ALocalMap* _pLocalMap, FVector2D _location_Start, FVector2D _location_End, const FHHM_Parameter_PathFind& _pathfind_Params) const;




};
