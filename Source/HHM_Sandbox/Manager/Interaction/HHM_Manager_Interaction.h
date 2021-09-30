// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/HHM_Manager.h"

#include "Data/Interaction/HHM_Data_Interaction_Base.h"	//Áö¿öµµµÊ
#include "Data/Interaction/HHM_Data_Interaction.h"

#include "HHM_Manager_Interaction.generated.h"



/**
 * Contains all the base datas of registered interactions
 * (Note : Register process were hard-coded.)
 */
UCLASS()
class HHM_SANDBOX_API AHHM_Manager_Interaction : public AHHM_Manager
{
	GENERATED_BODY()

public:
	AHHM_Manager_Interaction();



public:
	UPROPERTY()
		TMap<int32, FHHM_Data_Interaction_Base>		m_Container_InteractionBaseData;



public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;



public:
	bool		Get_BaseData_Interaction(const FHHM_Data_Interaction_Base*& _pBaseData_Interaction_Return, int32 _id_Interaction);



private:
	UFUNCTION()
		void Register_AllInteractionBaseData();



	//Interact interface
public:
	static void Execute_Interact(FHHM_Data_Interaction& _data_Interaction);
};
