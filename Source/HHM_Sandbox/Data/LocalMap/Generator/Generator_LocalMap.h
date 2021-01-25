// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Header/Struct.h"
#include "Header/Struct_Tile.h"
#include "Header/Struct_LocalMap.h"

#include "Generator_LocalMap.generated.h"



class AHHM_Manager_Tile;



UCLASS()
class HHM_SANDBOX_API AGenerator_LocalMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGenerator_LocalMap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



public:
	static void Generate_DebugMap(FHHM_LocalMap_MapData& _mapData, const FHHM_MapInfo& mapInfo, const AHHM_Manager_Tile* pManager_Tile);

};
