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
	static bool		Convert_Index_To_Translation(FVector& _vec_Translation_Return, int32 _index, const FHHM_MapInfo& _mapInfo);	//Return position is not a center position but start position (Left bottom corner)
	static bool		Convert_IndexLocation_To_Translation(FVector& _vec_Translation_Return, const FVector2D& _vec_IndexLocation, const FHHM_MapInfo& _mapInfo);
	static bool		Convert_Translation_To_Index(int32& _index_Return, const FVector& _vec_Translation, const FHHM_MapInfo& _mapInfo);
	static bool		Convert_Translation_To_IndexLocation(FVector2D& _vec_IndexLocation_Return, const FVector& _translation, const FHHM_MapInfo& _mapInfo);

	//Those function's are use for tile render system. may be deleted at some point
	static bool		Convert_Index_To_Translation_Relative(FVector& _vec_Translation_Return, int32 _index, const FHHM_MapInfo& _mapInfo);
	//Those function's are use for tile render system. may be deleted at some point
	static bool		Convert_Translation_To_Index_Relative(int32& _index_Return, const FVector& _vec_Translation, const FHHM_MapInfo& _mapInfo);

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
