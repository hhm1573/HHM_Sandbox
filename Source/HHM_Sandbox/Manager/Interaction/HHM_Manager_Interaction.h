// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/HHM_Manager.h"

#include "Data/Interaction/HHM_Data_Interaction_Base.h"	//지워도됨
#include "Data/Interaction/HHM_Data_Interaction.h"
#include "Base/Component/InteractionHandler/HHM_Component_InteractionHandler.h"

#include "HHM_Manager_Interaction.generated.h"



/**
 * Contains all the base datas of registered interactions
 * (Note : Register process were hard-coded.)
 *
 * Act Class will get needed interaction data from this manager.
 */
UCLASS()
class HHM_SANDBOX_API AHHM_Manager_Interaction : public AHHM_Manager
{
	GENERATED_BODY()

public:
	AHHM_Manager_Interaction();



public:
	UPROPERTY()
		//HHM Note : Interaction ID와 Interaction Type의 구분은 Interaction ID가 좀더 세분화 된 것이라고 보면 된다.
		//풀숲과 상호작용을 하여 나무열매를 따는것과 제작대와 상호작용을 하여 제작을 하는것은 둘다 SelfToTile Interaction 이지만 각자 걸리는 시간 등이 다르기 때문에 ID로 추가 구분이 되는것.
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
	static void Execute_Interact(FHHM_Data_Interaction& _data_Interaction, UHHM_Component_InteractionHandler* _pInteractionHandler_Executer);



private:
	static void Interact_EntityToTile(FHHM_Data_Interaction& _data_Interaction, UHHM_Component_InteractionHandler* _pInteractionHandler_Executer);
	static void Interact_GetItemFromTile(FHHM_Data_Interaction& _data_Interaction, UHHM_Component_InteractionHandler* _pInteractionHandler_Executer);
};
