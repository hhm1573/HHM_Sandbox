// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HHM_TileEntity.generated.h"

UCLASS()
class HHM_SANDBOX_API AHHM_TileEntity : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHHM_TileEntity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



protected:
	/*UPROPERTY()
		FIntPoint	m_Index = FIntPoint::ZeroValue;*/
	UPROPERTY()
		class ALocalMap*	m_pLocalMap = nullptr;
	UPROPERTY()
		int32				m_Index_Horizontal = -1;
	UPROPERTY()
		int32				m_Index_Vertical = -1;
	UPROPERTY()
		int32				m_Tile_ID = -1;
	UPROPERTY()
		int32				m_Tile_SubID = -1;



public:
	virtual bool Initialize_TileEntity(class AHHM_Tile* _pTile, class ALocalMap* _pLocalMap, int32 _index_Horizontal, int32 _index_Vertical);
};
