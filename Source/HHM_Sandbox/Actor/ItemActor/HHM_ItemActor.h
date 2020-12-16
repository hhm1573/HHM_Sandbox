// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HHM_ItemActor.generated.h"

UCLASS()
class HHM_SANDBOX_API AHHM_ItemActor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AHHM_ItemActor();



private:
	class UStaticMeshComponent* m_pComponent_StaticMesh = nullptr;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



public:
	//Set Item Data

};
