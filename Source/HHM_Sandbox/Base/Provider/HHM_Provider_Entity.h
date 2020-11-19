// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HHM_Provider_Entity.generated.h"

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API UHHM_Provider_Entity : public UObject
{
	GENERATED_BODY()

public:
	UHHM_Provider_Entity();

private:
	TMap<int32, UClass*> m_Container_Registered_Entity;

public:
	class AHHM_Entity* Spawn_Entity_By_ID(int32 _id, FVector& _location);

public:
	UFUNCTION(BlueprintCallable)
		bool Register_Entity(int32 _id_ToRegist, UClass* _pClass);
	
};
