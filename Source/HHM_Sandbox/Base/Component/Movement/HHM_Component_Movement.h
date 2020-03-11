// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"

#include "Header/Struct_Pathfinder.h"

#include "HHM_Component_Movement.generated.h"

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API UHHM_Component_Movement : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	UHHM_Component_Movement();

protected:
	int32 m_EntitySize_Horizontal = 1;
	int32 m_EntitySize_Vertical = 1;

	int32 m_MaxJumpHeight = 0;
	int32 m_MaxFallHeight = 0;
	int32 m_MaxHorizontalJumpLength = 0;

	float m_CurrentSpeed = 0.0f; //Tile Per Sec

	TArray<FHHM_PathNodeData>	m_FollowingPath;

public:
	virtual void	BeginPlay() override;
	virtual void	TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	bool			MoveToLocation(int32 _index_Horizontal, int32 _index_Vertical);

protected:
	virtual void	FollowPath(void);
	
	
};
