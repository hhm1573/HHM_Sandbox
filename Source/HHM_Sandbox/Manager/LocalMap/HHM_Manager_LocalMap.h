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
	const class AHHM_Manager_LocalMap*		m_pManager_LocalMap = nullptr;


	
private:
	UPROPERTY()
		TMap<int32, class ALocalMap*>	m_Container_LocalMap;
	UPROPERTY()
		int32							m_AvailiableIndex_LocalMap = 0;

	//Represent the total reserved elements on container
	UPROPERTY()
		int32							m_Size_Container_Entity = 0;
	UPROPERTY()
		TArray<class AHHM_Entity*>		m_Container_Entity;
	UPROPERTY()
		TArray<int32>					m_Container_AvailiableIndex_Entity; 



public:
	virtual void BeginPlay() override;

private:
	void Initialize_Manager_LocalMap(void);


	
public:
	bool Create_LocalMap(FHHM_LocalMapConstructionData _mapConstructionData, FHHM_LocalMapConstructionResult& _mapConstructionResult);

private:
	bool	Initialize_MapData(const FHHM_MapInfo& _mapInfo, int32 _index_LocalMap, TArray<FHHM_TileData> _ref_Container_TileData);



private:
	int32	Find_AvailiableIndex_LocalMap(void);
	void	Resize_Container_AvailiableIndex_Entity(void);



private:
	int32	Register_Entity(int32 _index_LocalMap, class AHHM_Entity* _pEntity); //Check entity has id. if not, do register
	void	DeRegister_Entity(int32 _index_LocalMap, class AHHM_Entity* _pEntity);
};
