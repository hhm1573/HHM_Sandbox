// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_PFPriorityQueue.h"

HHM_PFPriorityQueue::HHM_PFPriorityQueue()
{
	m_Container_NodeInfo.Empty();
	m_Container_NodeInfo.AddDefaulted();	//Fill 0 index as dummy data
}

HHM_PFPriorityQueue::~HHM_PFPriorityQueue()
{
}



bool HHM_PFPriorityQueue::Is_Empty(void) {
	const int32 Size_Container = m_Container_NodeInfo.Num();
	return Size_Container > 1 ? false : true;
}

int32 HHM_PFPriorityQueue::Get_Index_Parent(int32 _index_Child) {
	const int32 Size_Container = m_Container_NodeInfo.Num();

	if (_index_Child >= Size_Container) {	//check is input index is out ranged
		return 0;
	}

	return int32(_index_Child / 2);
}

int32 HHM_PFPriorityQueue::Get_Index_Child_HigherPriority(int32 _index_Parent) {
	const int32 Size_Container = m_Container_NodeInfo.Num();
	
	if (_index_Parent >= Size_Container) {
		return -1;	//Input index is out ranged
	}

	const int32 Index_LeftChild = _index_Parent * 2;
	const int32 Index_RightChild = _index_Parent * 2 + 1;

	if (Index_LeftChild >= Size_Container) {		//Parent has no child
		return 0;
	}
	else if (Index_RightChild >= Size_Container) {	//Parent has only one child (Left one)
		return Index_LeftChild;
	}
	else {											//Parent has two children
		float	Cost_F_LeftChild = m_Container_NodeInfo[Index_LeftChild].Cost_F;
		float	Cost_F_RightChild = m_Container_NodeInfo[Index_RightChild].Cost_F;

		return Cost_F_LeftChild >= Cost_F_RightChild ? Index_RightChild : Index_LeftChild;
	}
}



bool HHM_PFPriorityQueue::Add(FHHM_PathFinderNodeInfo& _nodeInfo) {
	int32 Index_Last = m_Container_NodeInfo.Add(_nodeInfo);
	int32 Index_Parent = HHM_PFPriorityQueue::Get_Index_Parent(Index_Last);

	const float Cost_F_Add = _nodeInfo.Cost_F;

	while (Index_Parent != 0) {
		const float Cost_F_Parent = m_Container_NodeInfo[Index_Parent].Cost_F;

		if (Cost_F_Add > Cost_F_Parent) {
			break;
		}

		m_Container_NodeInfo[Index_Last] = m_Container_NodeInfo[Index_Parent];

		Index_Last = Index_Parent;
		Index_Parent = HHM_PFPriorityQueue::Get_Index_Parent(Index_Last);
	}

	m_Container_NodeInfo[Index_Last] = _nodeInfo;
	return true;



	/*int32 Index_Last = m_Container_NodeInfo.Add(_nodeInfo);
	int32 Index_Parent = HHM_PFPriorityQueue::Get_Index_Parent(Index_Last);

	while (Index_Parent != 0) {
		const float Cost_F_Last = m_Container_NodeInfo[Index_Last].Cost_F;
		const float Cost_F_Parent = m_Container_NodeInfo[Index_Parent].Cost_F;

		if (Cost_F_Last > Cost_F_Parent) {
			break;
		}

		m_Container_NodeInfo[Index_Last] = m_Container_NodeInfo[Index_Parent];

		Index_Last = Index_Parent;
		Index_Parent = HHM_PFPriorityQueue::Get_Index_Parent(Index_Last);
	}

	m_Container_NodeInfo[Index_Last] = _nodeInfo;
	return true;*/
}

bool HHM_PFPriorityQueue::Add(float _cost_F, int32 _index, EHHM_MoveType _eMoveType) {
	FHHM_PathFinderNodeInfo NodeInfo = FHHM_PathFinderNodeInfo(_index, _eMoveType, _cost_F);
	return HHM_PFPriorityQueue::Add(NodeInfo);
}

bool HHM_PFPriorityQueue::Remove(FHHM_PathFinderNodeInfo& _nodeInfo_Out) {
	const bool IsQueueEmpty = HHM_PFPriorityQueue::Is_Empty();
	if (IsQueueEmpty == true) {
		return false;
	}

	const int32 Size_Container = m_Container_NodeInfo.Num();

	if (Size_Container > 20) {
		int32 i = 0;
	}

	FHHM_PathFinderNodeInfo NodeInfo_Return = m_Container_NodeInfo[1];
	//m_Container_NodeInfo[1] = m_Container_NodeInfo[Size_Container - 1];
	FHHM_PathFinderNodeInfo NodeInfo_Last = m_Container_NodeInfo[Size_Container - 1];
	m_Container_NodeInfo.RemoveAt(Size_Container - 1);

	if (Size_Container == 2) {
		_nodeInfo_Out = NodeInfo_Return;
		return true;
	}

	int32 Index_Current = 1;	//since index 0 is reserved as dummy data, index starts from 1
	int32 Index_HigherPriorityChild = HHM_PFPriorityQueue::Get_Index_Child_HigherPriority(Index_Current);

	const float Cost_F_Last = NodeInfo_Last.Cost_F;

	while (Index_HigherPriorityChild > 0) {
		//const float Cost_F_Current = m_Container_NodeInfo[Index_Current].Cost_F;
		const float Cost_F_Child = m_Container_NodeInfo[Index_HigherPriorityChild].Cost_F;

		if (Cost_F_Last < Cost_F_Child) {
			break;
		}

		m_Container_NodeInfo[Index_Current] = m_Container_NodeInfo[Index_HigherPriorityChild];

		Index_Current = Index_HigherPriorityChild;
		Index_HigherPriorityChild = HHM_PFPriorityQueue::Get_Index_Child_HigherPriority(Index_Current);
	}

	m_Container_NodeInfo[Index_Current] = NodeInfo_Last;

	_nodeInfo_Out = NodeInfo_Return;
	return true;
}