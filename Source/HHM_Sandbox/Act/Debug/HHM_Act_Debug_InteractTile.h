// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Act/HHM_Act.h"
#include "HHM_Act_Debug_InteractTile.generated.h"

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API UHHM_Act_Debug_InteractTile : public UHHM_Act
{
	GENERATED_BODY()

public:
	UHHM_Act_Debug_InteractTile();



public:
	virtual bool Begin_Act(FHHM_Data_Action& _data_Action_Return) override;
	
};
