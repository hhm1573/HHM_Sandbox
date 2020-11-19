// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/HHM_Manager.h"

#include "Header/Macro.h"
#include "Header/Struct.h"
#include "Header/Struct_Tile.h"
#include "Header/Struct_LocalMap.h"

#include "HHM_Manager_LocalMap.generated.h"

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API AHHM_Manager_LocalMap : public AHHM_Manager
{
	GENERATED_BODY()

public:
	AHHM_Manager_LocalMap();



private:
	const class AHHM_Manager_Tile*		m_pManager_Tile = nullptr;


	
private:
	UPROPERTY(VisibleAnywhere)
		TMap<int32, class ALocalMap*>	m_Container_LocalMap;
	UPROPERTY()
		int32							m_AvailiableIndex_LocalMap = 0;



public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void	Initialize_Manager_LocalMap(void);


	
public:
	UFUNCTION(BlueprintCallable)
		bool				Create_LocalMap(FHHM_LocalMapConstructionData _mapConstructionData, FHHM_LocalMapConstructionResult& _mapConstructionResult);
	UFUNCTION(BlueprintCallable)
		bool				Destroy_LocalMap(int32 _id_LocalMap);
	UFUNCTION(BlueprintCallable)
	class ALocalMap*		Get_LocalMap(int32 _index_LocalMap);
	const class ALocalMap*	Get_LocalMap_const(int32 _index_LocalMap) const;

private:
	bool	Construct_MapData_Empty(FHHM_LocalMap_MapData* _mapData, const FHHM_MapInfo& _mapInfo);
	bool	Construct_MapData_TestMap(FHHM_LocalMap_MapData* _mapData, const FHHM_MapInfo& _mapInfo);
	// HHM Note : Add function that Load Local Map from save.



private:
	int32	Find_AvailiableIndex_LocalMap(void);



private:
	int32	Register_Entity(int32 _index_LocalMap, class AHHM_Entity* _pEntity); //Check entity has id. if not, do register
	void	DeRegister_Entity(int32 _index_LocalMap, class AHHM_Entity* _pEntity);
};
