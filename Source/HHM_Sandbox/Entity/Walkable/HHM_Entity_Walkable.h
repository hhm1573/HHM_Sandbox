// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/HHM_Entity.h"
#include "HHM_Entity_Walkable.generated.h"



class UHHM_Component_Movement;



/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API AHHM_Entity_Walkable : public AHHM_Entity
{
	GENERATED_BODY()

public:
	AHHM_Entity_Walkable();

private:
	UPROPERTY(Category = "HHM_Entity", VisibleAnywhere)
		UHHM_Component_Movement* m_pComponent_Movement = nullptr;

public:
	//Add Move Function
	UFUNCTION(BlueprintCallable)
		bool	Order_MoveToLocation(int32 _index_Horizontal, int32 _index_Vertical);

public:
	UFUNCTION(BlueprintCallable)
		float	Get_MoveTimer();	// HHM Note : Fix this functions name something like "Get_MoveComponent_MoveTimer", "MoveComponent_Get_MoveTimer" or "Component_Movement_Get_MoveTimer" etc.
};
