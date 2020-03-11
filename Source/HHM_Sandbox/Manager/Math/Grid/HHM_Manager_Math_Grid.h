// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/HHM_Manager.h"

#include "Header/Struct.h"

#include "HHM_Manager_Math_Grid.generated.h"

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API AHHM_Manager_Math_Grid : public AHHM_Manager
{
	GENERATED_BODY()

public:
	static FVector	Convert_IndexToTranslation(const FHHM_MapInfo& mapInfo, int32 index);	//Return position is not a center position but start position (Left bottom corner)
	static int32	Convert_TranslationToIndex(const FHHM_MapInfo& mapInfo, const FVector& translation);

	/*
	Get Horizontal and Vertical index and combine it as an index use for map array.
	It Does Not do valid check
	*/
	static int32	Index_Combine(int32 _index_Horizontal, int32 _index_Vertical, const FHHM_MapInfo& _mapInfo);
	/*
	Get index and extract Horizontal and Vertical index from it.
	it does valid check of horizontal and vertical index based on inserted mapInfo
	and return false if return value is out of range
	*/
	static bool		Index_Seperate(int32& _index_Horizontal, int32& _index_Vertical, int32 _index, const FHHM_MapInfo& _mapInfo);
	/*
	Get index and extract Horizontal and Vertical index from it.
	it does valid check of horizontal and vertical index based on inserted mapInfo
	and return false if return value is out of range
	*/
	static bool		Index_Seperate(FVector2D& _vecLocation_Return, int32 _index, const FHHM_MapInfo& _mapInfo);

};
