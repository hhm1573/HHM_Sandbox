// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Header/Struct_Pathfinder.h"

/**
 * 
 */
class HHM_SANDBOX_API HHM_PFPriorityQueue
{
public:
	HHM_PFPriorityQueue();
	~HHM_PFPriorityQueue();

private:
	TArray<FHHM_PathFinderNodeInfo>		m_Container_NodeInfo;

public:
	bool	Is_Empty(void);
	int32	Get_Index_Parent(int32 _index_Child);
	int32	Get_Index_Child_HigherPriority(int32 _index_Parent);

	bool	Add(FHHM_PathFinderNodeInfo& _nodeInfo);
	bool	Add(float _cost_F, int32 _index, EHHM_MoveType _eMoveType);
	/*
	'_nodeInfo_Out' ) Reference of nodeinfo. removed element will be saved into this reference.
	Return ) return true if remove is succeed.
	*/
	bool	Remove(FHHM_PathFinderNodeInfo& _nodeInfo_Out);

};
