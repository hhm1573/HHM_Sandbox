// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Data/Item/ItemData/HHM_ItemData.h"

#include "HHM_ItemActor.generated.h"

UCLASS()
class HHM_SANDBOX_API AHHM_ItemActor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AHHM_ItemActor();



private:
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent*		m_pComponent_StaticMesh = nullptr;

	UHHM_ItemData*		m_pItemData = nullptr;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



public:
	//Set Item Data
	bool Set_ItemData(UHHM_ItemData* _pItemData, class UMaterialInterface* _material, FVector2D _actorSize);
	void Add_Force(FVector _force);

};
