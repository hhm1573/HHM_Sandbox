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
	UPROPERTY()
		TMap<int32, class ALocalMap*>	m_Container_LocalMap;
	UPROPERTY()
		int32							m_AvailiableIndex_LocalMap = 0;

	//Represent the total reserved elements on container
	UPROPERTY()
		int32							m_Size_Container_Entity = 0;	//HHM Note : Move entity container to LocalMap
	UPROPERTY()
		TArray<class AHHM_Entity*>		m_Container_Entity;
	UPROPERTY()
		TArray<int32>					m_Container_AvailiableIndex_Entity; //Resize if total number of entity is over container.



public:
	virtual void BeginPlay() override;

private:
	void	Initialize_Manager_LocalMap(void);


	
public:
	bool					Create_LocalMap(FHHM_LocalMapConstructionData _mapConstructionData, FHHM_LocalMapConstructionResult& _mapConstructionResult);
	UFUNCTION(BlueprintCallable)
	class ALocalMap*		Get_LocalMap(int32 _index_LocalMap);
	const class ALocalMap*	Get_LocalMap_const(int32 _index_LocalMap) const;

private:
	bool	Construct_MapData_Empty(FHHM_LocalMap_MapData* _mapData, const FHHM_MapInfo& _mapInfo);
	bool	Construct_MapData_TestMap(FHHM_LocalMap_MapData* _mapData, const FHHM_MapInfo& _mapInfo);
	// HHM Note : Add function that Load Local Map from save.



private:
	int32	Find_AvailiableIndex_LocalMap(void);
	void	Resize_Container_AvailiableIndex_Entity(void);



private:
	int32	Register_Entity(int32 _index_LocalMap, class AHHM_Entity* _pEntity); //Check entity has id. if not, do register
	void	DeRegister_Entity(int32 _index_LocalMap, class AHHM_Entity* _pEntity);
};
