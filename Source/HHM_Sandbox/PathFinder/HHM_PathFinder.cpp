// Fill out your copyright notice in the Description page of Project Settings.

#include "HHM_PathFinder.h"

#include "Header/Struct.h"
#include "Header/Struct_Pathfinder.h"
#include "PathFinder/HHM_PFPriorityQueue.h"

#include "Data/LocalMap/LocalMap.h"
#include "Manager/Math/Grid/HHM_Manager_Math_Grid.h"





TArray<FHHM_PathNodeData> UHHM_PathFinder::Search_Path(const ALocalMap * _pLocalMap, FVector2D _location_Start, FVector2D _location_End, int32 _entitySize_Width, int32 _entitySize_Height, int32 _maxJumpLength, int32 _maxFallLength, int32 _maxHorizontalJumpLength) {

	TArray<FHHM_PathNodeData> Container_PathNodeData_Return;	//Container of node locations as Vector2D for return value
	Container_PathNodeData_Return.Empty();

	//Basic Valid Check
	if (_pLocalMap == nullptr) {
		//Exception
		return Container_PathNodeData_Return;
	}

	//Get Map Information and check start location
	const FHHM_MapInfo	MapInfo = _pLocalMap->Get_MapInfo();
	const int32	Num_Map = MapInfo.MapSize_Horizontal * MapInfo.MapSize_Vertical;
	if (_location_Start.X < 0 || _location_Start.X >= MapInfo.MapSize_Horizontal
		|| _location_Start.Y < 0 || _location_Start.Y >= MapInfo.MapSize_Vertical) {
		//Exception
		return Container_PathNodeData_Return;
	}

	//Check Start Location and get index of start
	if (_location_Start.X < 0 || _location_Start.X >= MapInfo.MapSize_Horizontal
		|| _location_Start.Y < 0 || _location_Start.Y >= MapInfo.MapSize_Vertical) {
		//Exception
		return Container_PathNodeData_Return;
	}
	const int32	Index_Start = AHHM_Manager_Math_Grid::Index_Combine(_location_Start.X, _location_Start.Y, MapInfo);
	if (Index_Start < 0 || Index_Start >= Num_Map) {
		//Exception
		return Container_PathNodeData_Return;
	}

	//Check End Location and get index of end's then save it as variable. this variable will be used on finishing process
	if (_location_End.X < 0 || _location_End.X >= MapInfo.MapSize_Horizontal
		|| _location_End.Y < 0 || _location_End.Y >= MapInfo.MapSize_Vertical) {
		//Exception
		return Container_PathNodeData_Return;
	}
	const int32 Index_End = AHHM_Manager_Math_Grid::Index_Combine(_location_End.X, _location_End.Y, MapInfo);
	if (Index_End < 0 || Index_End >= Num_Map) {
		//Exception
		return Container_PathNodeData_Return;
	}

	//if start location or end location is not passable, return
	bool Passable_StartLocation = _pLocalMap->IsLocationPassable(Index_Start);
	bool Passable_EndLocation = _pLocalMap->IsLocationPassable(Index_End);

	if (Passable_StartLocation == false || Passable_EndLocation == false) {
		//Exception
		return Container_PathNodeData_Return;
	}

	//if Start location is not on ground, return
	bool IsOnGround_StartLocation = _pLocalMap->IsOnGround(Index_Start);
	if (IsOnGround_StartLocation == false) {
		//Exception
		return Container_PathNodeData_Return;
	}

	//if End location is not on ground, return
	bool IsOnGround_EndLocation = _pLocalMap->IsOnGround(Index_End);
	if (IsOnGround_EndLocation == false) {
		//Exception
		return Container_PathNodeData_Return;
	}



	//Declare some basic const value
	const float		UnitDistance_Tile = 10.0f;
	const int8		StatusValue_Open = 1;
	const int8		StatusValue_Closed = -1;
	const int32		DirectionDistance[4] = { MapInfo.MapSize_Horizontal, -MapInfo.MapSize_Horizontal, -1, 1 };	//Up Down Left Right

	//Initialize Basic Container
	TArray<FHHM_PathFinderNode>			Container_Node[(uint8)EHHM_MoveType::MT_End];
	HHM_PFPriorityQueue					List_Open;
	//TArray<FVector2D>				List_Closed;

	Container_Node[(uint8)EHHM_MoveType::MT_OnGround].Init(FHHM_PathFinderNode(), Num_Map);
	Container_Node[(uint8)EHHM_MoveType::MT_Jump].Init(FHHM_PathFinderNode(), Num_Map);
	Container_Node[(uint8)EHHM_MoveType::MT_Fall].Init(FHHM_PathFinderNode(), Num_Map);
	Container_Node[(uint8)EHHM_MoveType::MT_HorizontalJump_Left].Init(FHHM_PathFinderNode(), Num_Map);
	Container_Node[(uint8)EHHM_MoveType::MT_HorizontalJump_Right].Init(FHHM_PathFinderNode(), Num_Map);
	//List_Open.Empty();
	//List_Closed.Empty();



	//Create Start Node
	//const bool IsStartNodeOnGround = _pLocalMap->IsOnGround(Index_Start);

	FHHM_PathFinderNode	Node_Start = FHHM_PathFinderNode();
	Node_Start.Location_Parent = FVector2D(_location_Start.X, _location_Start.Y);
	Node_Start.Cost_G = 0.0f;
	Node_Start.Cost_H = 0.0f;
	Node_Start.Cost_F = Node_Start.Cost_G + Node_Start.Cost_H;
	//Node_Start.JumpValue = IsStartNodeOnGround == true ? 0 : _maxJumpLength;
	Node_Start.JumpValue = 0;
	Node_Start.Status = StatusValue_Open;

	Container_Node[(uint8)EHHM_MoveType::MT_OnGround][Index_Start] = Node_Start;

	FHHM_PathFinderNodeInfo NodeInfo_Start = FHHM_PathFinderNodeInfo(Index_Start, EHHM_MoveType::MT_OnGround, Node_Start.Cost_F);

	List_Open.Add(NodeInfo_Start);


	int		Counter_ProcessedNode = 0;
	bool			IsFound = false;
	EHHM_MoveType	eMoveType_End = EHHM_MoveType::MT_End;



	//Start Search
	while (List_Open.Is_Empty() == false) {

		//Find Node that has lowest F Cost
		FHHM_PathFinderNodeInfo	NodeInfo_LowestCost = FHHM_PathFinderNodeInfo();
		const bool				IsSucceed_FindLowest = List_Open.Remove(NodeInfo_LowestCost);
		if (IsSucceed_FindLowest == false) {
			//Exception
			return Container_PathNodeData_Return;
		}
		int32					Index_LowestCost = NodeInfo_LowestCost.Index;
		EHHM_MoveType			eMoveType_LowestCost = NodeInfo_LowestCost.eMoveType;

		int32					Index_Open_Lowest = 0;	//Index for pointing "Open Set"

		Container_Node[(uint8)eMoveType_LowestCost][Index_LowestCost].Status = StatusValue_Closed;

		//Save lowest cost node's location as variable for comparing work on neighbor node
		int32	Location_LowestCost_Horizontal = -1;
		int32	Location_LowestCost_Vertical = -1;
		if (AHHM_Manager_Math_Grid::Index_Seperate(Location_LowestCost_Horizontal, Location_LowestCost_Vertical, Index_LowestCost, MapInfo) == false) {
			//Exception
			break;
		}

		//Check if lowest cost node is End node
		FVector2D	Location_LowestCost = FVector2D(Location_LowestCost_Horizontal, Location_LowestCost_Vertical);
		if (Location_LowestCost == _location_End) {
			Container_Node[(uint8)eMoveType_LowestCost][Index_LowestCost].Status = StatusValue_Closed;
			IsFound = true;
			eMoveType_End = eMoveType_LowestCost;
			break;
		}



		if (Counter_ProcessedNode > 1000) {
			int32 breakpoint = 0;
		}



		//Iterate Neighbors of lowest cost node
		for (int32 index_Dir = 0; index_Dir < 4; ++index_Dir) {

			//Get Neighbors index and check is it valid
			int32	Index_Neighbor = Index_LowestCost + DirectionDistance[index_Dir];

			int32	Location_Neighbor_Horizontal = -1;
			int32	Location_Neighbor_Vertical = -1;
			if (AHHM_Manager_Math_Grid::Index_Seperate(Location_Neighbor_Horizontal, Location_Neighbor_Vertical, Index_Neighbor, MapInfo) == false)
				continue;

			//if neighbor is too far from lowest cost (Current) node, skip it
			int32	Distance_Neighbor_Horizontal = FMath::Abs(Location_LowestCost_Horizontal - Location_Neighbor_Horizontal);
			int32	Distance_Neighbor_Vertical = FMath::Abs(Location_LowestCost_Vertical - Location_Neighbor_Vertical);
			if (Distance_Neighbor_Horizontal > 1 || Distance_Neighbor_Vertical > 1) {
				continue;
			}



			//check Neighbor node is impassable
			bool	IsLocationPassable = _pLocalMap->IsLocationPassable(Index_Neighbor);
			if (IsLocationPassable == false) {
				continue;
			}

			//Get New Jump Value for neighbor node
			bool	IsOnGround = _pLocalMap->IsOnGround(Index_Neighbor);

			int8			JumpValue_Neighbor = Container_Node[(uint8)eMoveType_LowestCost][Index_LowestCost].JumpValue;
			int8			HorizontalJumpValue_Neighbor = Container_Node[(uint8)eMoveType_LowestCost][Index_LowestCost].HorizontalJumpValue;
			EHHM_MoveType	eMoveType_Neighbor = eMoveType_LowestCost;

			bool			MoveOnAir_Neighbor = Container_Node[(uint8)eMoveType_LowestCost][Index_LowestCost].MoveOnAir;

			if (Index_Neighbor == 207 || Index_Neighbor == 177 || Index_Neighbor == 215) {
				int breakpoint = 0;
			}

			switch (index_Dir) {
			case 0:		//Up
				if (eMoveType_LowestCost == EHHM_MoveType::MT_OnGround || eMoveType_LowestCost == EHHM_MoveType::MT_Jump) {
					++JumpValue_Neighbor;
					eMoveType_Neighbor = EHHM_MoveType::MT_Jump;
				}
				else {
					continue;
				}
				break;

			case 1:		//Down
				if (HorizontalJumpValue_Neighbor == 1) {

					if (eMoveType_LowestCost == EHHM_MoveType::MT_Jump) {
						continue;
					}

					if (IsOnGround == true) {
						JumpValue_Neighbor = 0;
						HorizontalJumpValue_Neighbor = 0;
						eMoveType_Neighbor = EHHM_MoveType::MT_OnGround;
					}
					else {
						--JumpValue_Neighbor;
						eMoveType_Neighbor = EHHM_MoveType::MT_Fall;
					}
				}
				else {
					continue;
				}
				break;

			case 2:		//Left
			case 3:		//Right
				if (eMoveType_LowestCost == EHHM_MoveType::MT_Jump) {
					if (IsOnGround == true) {
						JumpValue_Neighbor = 0;
						HorizontalJumpValue_Neighbor = 0;
						eMoveType_Neighbor = EHHM_MoveType::MT_OnGround;
					}
					else {
						continue;
					}
				}
				switch (eMoveType_LowestCost) {
				case EHHM_MoveType::MT_Jump:
					if (IsOnGround == true) {
						JumpValue_Neighbor = 0;
						HorizontalJumpValue_Neighbor = 0;
						eMoveType_Neighbor = EHHM_MoveType::MT_OnGround;
					}
					else {
						continue;
					}
					break;

				case EHHM_MoveType::MT_Fall:
					continue;
					break;

				case EHHM_MoveType::MT_OnGround:
				case EHHM_MoveType::MT_HorizontalJump_Left:
				case EHHM_MoveType::MT_HorizontalJump_Right:
					if (IsOnGround == true) {
						JumpValue_Neighbor = 0;
						HorizontalJumpValue_Neighbor = 0;
						eMoveType_Neighbor = EHHM_MoveType::MT_OnGround;
					}
					else {
						++HorizontalJumpValue_Neighbor;

						if (index_Dir == 2) {	//Left
							eMoveType_Neighbor = EHHM_MoveType::MT_HorizontalJump_Left;
						}
						else {
							eMoveType_Neighbor = EHHM_MoveType::MT_HorizontalJump_Right;
						}

						//Skip if horizontal jump direction changed during horizontal jump
						if (eMoveType_LowestCost != EHHM_MoveType::MT_OnGround && eMoveType_LowestCost != eMoveType_Neighbor) {
							continue;
						}
					}
					break;

				default:	//Just in case
					//Exception
					return Container_PathNodeData_Return;
				}

				break;

			default:	//Just in case
				if (index_Dir >= 4) {
					//Exception
					return Container_PathNodeData_Return;
				}
			}



			//if node is too high or too low, skip that node
			if (JumpValue_Neighbor > _maxJumpLength) {		//remove '=' to make it can move 1 more tile than max jump/fall length
				if (Location_Neighbor_Vertical > Location_LowestCost_Vertical) {
					continue;
				}
			}
			else if (JumpValue_Neighbor <= -_maxFallLength) {
				if (Location_Neighbor_Vertical < Location_LowestCost_Vertical) {
					continue;
				}
			}

			//Check Horizontal jump
			if (HorizontalJumpValue_Neighbor > _maxHorizontalJumpLength) {
				if (Location_Neighbor_Horizontal != Location_LowestCost_Horizontal) {
					continue;	//If Current node's jumpvalue is over max jump length, continue.
				}
			}



			//Check if neighbor node is marked as closed
			if (Container_Node[(uint8)eMoveType_Neighbor][Index_Neighbor].Status == StatusValue_Closed) {

				//if node is representing vertical movement(Jump or fall) and it is shorter path than old path, don't skip it
				bool IsVerticallyShorterPath = false;

				if ((eMoveType_Neighbor == EHHM_MoveType::MT_Fall && JumpValue_Neighbor > Container_Node[(uint8)eMoveType_Neighbor][Index_Neighbor].JumpValue)
					|| (eMoveType_Neighbor == EHHM_MoveType::MT_Jump && JumpValue_Neighbor < Container_Node[(uint8)eMoveType_Neighbor][Index_Neighbor].JumpValue)) {

					IsVerticallyShorterPath = true;
				}
				
				if (IsVerticallyShorterPath == false) {
					continue;
				}
			}



			//**************** Get Other Costs of neighbor node ****************

			FVector2D	Vec_Neighbor = FVector2D(Location_Neighbor_Horizontal, Location_Neighbor_Vertical);

			//G Cost
			float		Cost_G_Neighbor = Container_Node[(uint8)eMoveType_LowestCost][Index_LowestCost].Cost_G + FMath::Abs(FVector2D::Distance(Vec_Neighbor, Location_LowestCost));

			//H Cost
			float		Cost_H_Neighbor = FMath::Abs(FVector2D::Distance(Vec_Neighbor, _location_End));

			//F Cost
			float		Cost_F_Neighbor = Cost_G_Neighbor + Cost_H_Neighbor;

			//******************************************************************



			//Condition check for update node
			const bool IsNewNodeIsShorter = Cost_G_Neighbor < Container_Node[(uint8)eMoveType_Neighbor][Index_Neighbor].Cost_G ? true : false;
			if (IsNewNodeIsShorter == true || Container_Node[(uint8)eMoveType_Neighbor][Index_Neighbor].Status != StatusValue_Open) {

				//Update node

				Container_Node[(uint8)eMoveType_Neighbor][Index_Neighbor].JumpValue = JumpValue_Neighbor;
				Container_Node[(uint8)eMoveType_Neighbor][Index_Neighbor].HorizontalJumpValue = HorizontalJumpValue_Neighbor;
				Container_Node[(uint8)eMoveType_Neighbor][Index_Neighbor].MoveOnAir = MoveOnAir_Neighbor;

				Container_Node[(uint8)eMoveType_Neighbor][Index_Neighbor].Cost_G = Cost_G_Neighbor;
				Container_Node[(uint8)eMoveType_Neighbor][Index_Neighbor].Cost_H = Cost_H_Neighbor;
				Container_Node[(uint8)eMoveType_Neighbor][Index_Neighbor].Cost_F = Cost_F_Neighbor;

				Container_Node[(uint8)eMoveType_Neighbor][Index_Neighbor].Location_Parent = FVector2D(Location_LowestCost_Horizontal, Location_LowestCost_Vertical);
				Container_Node[(uint8)eMoveType_Neighbor][Index_Neighbor].eMoveType_Parent = eMoveType_LowestCost;



				if (Container_Node[(uint8)eMoveType_Neighbor][Index_Neighbor].Status != StatusValue_Open) {

					Container_Node[(uint8)eMoveType_Neighbor][Index_Neighbor].Status = StatusValue_Open;

					FHHM_PathFinderNodeInfo NodeInfo_Neighbor = FHHM_PathFinderNodeInfo(Index_Neighbor, eMoveType_Neighbor, Container_Node[(uint8)eMoveType_Neighbor][Index_Neighbor].Cost_F);
					List_Open.Add(NodeInfo_Neighbor);

				}

			}

		}

		++Counter_ProcessedNode;
	}


	//Finishing path finding. construct return nodes set
	if (IsFound == true) {

		if (eMoveType_End == EHHM_MoveType::MT_End) {
			//Check move type, just in case. it may not occur any problem if be removed
			return Container_PathNodeData_Return;
		}

		FVector2D		Location_Before = _location_End;
		int32			Index_Before = Index_End;
		EHHM_MoveType	eMoveType_Before = eMoveType_End;

		FVector2D		Location_Current = _location_End;
		int32			Index_Current = Index_End;
		EHHM_MoveType	eMoveType_Current = eMoveType_End;

		FVector2D		Location_Parent = Container_Node[(uint8)eMoveType_End][Index_End].Location_Parent;
		int32			Index_Parent = AHHM_Manager_Math_Grid::Index_Combine(Location_Parent.X, Location_Parent.Y, MapInfo);
		EHHM_MoveType	eMoveType_Parent = Container_Node[(uint8)eMoveType_End][Index_End].eMoveType_Parent;

		int32			MoveValue = 0;

		while (Location_Current != Location_Parent) {
			//Container_NodeLoc_Return.Add(Location_Current);

			//----------------| Filtering work |----------------//

			if (Index_Current == Index_End		//Current Node is end node
				|| (eMoveType_Before != EHHM_MoveType::MT_OnGround && eMoveType_Current == EHHM_MoveType::MT_OnGround)	//(Start Jumping)Before node is not on ground while current node is on ground
				|| (eMoveType_Parent != EHHM_MoveType::MT_OnGround && eMoveType_Current == EHHM_MoveType::MT_OnGround)	//(Landing)
				) {	

				int32			Index_FilteredNode = Index_Current;
				EHHM_MoveType	eMoveType_FilteredNode = eMoveType_Parent;
				FVector2D		Location_FilteredNode = Location_Current;

				//Horizontal Jump Fix
				//패스파인딩 과정에서 낙하를 실행할경우 1.낙하하는 방향으로 멀리뛰기 수행 2.멀리뛰기 실행후 바로 밑에 (이동거리가 1일경우) 지면이 있을경우 OnGround 상태로 변환됨
				//의 과정을 거침으로 인해 탐색경로를 구축하는 과정에서 (바로 아래의 Container_PathNodeData_Return.Add 부분) 1칸 낙하하는 경우에 이동 방식이 잘못 입력됨
				if ((eMoveType_Parent == EHHM_MoveType::MT_HorizontalJump_Left || eMoveType_Parent == EHHM_MoveType::MT_HorizontalJump_Right)
					&& MoveValue == 1) {
					eMoveType_FilteredNode = EHHM_MoveType::MT_Fall;
				}

				//Create NodeData and store it into return container
				FHHM_PathNodeData	PathNodeData = FHHM_PathNodeData(-1, Index_FilteredNode, eMoveType_FilteredNode, MoveValue, Location_FilteredNode);
				Container_PathNodeData_Return.Add(PathNodeData);

				MoveValue = 0;
			}

			//--------------------------------------------------//

			Location_Before = Location_Current;
			Index_Before = Index_Current;
			eMoveType_Before = eMoveType_Current;

			Location_Current = Location_Parent;
			Index_Current = Index_Parent;
			eMoveType_Current = eMoveType_Parent;

			Location_Parent = Container_Node[(uint8)eMoveType_Current][Index_Current].Location_Parent;
			Index_Parent = AHHM_Manager_Math_Grid::Index_Combine(Location_Parent.X, Location_Parent.Y, MapInfo);
			eMoveType_Parent = Container_Node[(uint8)eMoveType_Current][Index_Current].eMoveType_Parent;

			++MoveValue;
		}

		//Since last location (Start location) is not added add it manually
		FHHM_PathNodeData	PathNodeData_Start = FHHM_PathNodeData(Index_Current, Index_Current, eMoveType_Current, MoveValue, Location_Current);
		Container_PathNodeData_Return.Add(PathNodeData_Start);

		//Reverse Array
		TArray<FHHM_PathNodeData>	Container_PathNodeData_Temp;
		for (int32 index_PathNodeData = Container_PathNodeData_Return.Num() - 1; index_PathNodeData >= 0; --index_PathNodeData) {
			Container_PathNodeData_Temp.Add(Container_PathNodeData_Return[index_PathNodeData]);
		}

		Container_PathNodeData_Return = Container_PathNodeData_Temp;



		//Fix the ordering of MoveValue and Set Start Index of nodedata as valid value
		int32 Size_Contaier_Return = Container_PathNodeData_Return.Num();
		if (Size_Contaier_Return > 1) {
			for (int32 Index_Fixing = Size_Contaier_Return - 1; Index_Fixing > 0; --Index_Fixing) {
				Container_PathNodeData_Return[Index_Fixing].MoveValue = Container_PathNodeData_Return[Index_Fixing - 1].MoveValue;
				//Container_PathNodeData_Return[Index_Fixing - 1].MoveValue = 0;
				Container_PathNodeData_Return[Index_Fixing].Index_Start = Container_PathNodeData_Return[Index_Fixing - 1].Index_Target;
			}

			Container_PathNodeData_Return[0].MoveValue = 0;
		}



		//Container_PathNodeData_Return = Container_PathNodeData_Temp;
	}



	return Container_PathNodeData_Return;
}
