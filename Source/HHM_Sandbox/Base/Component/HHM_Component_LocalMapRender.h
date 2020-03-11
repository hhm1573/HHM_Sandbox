// Fill out your copyright notice in the Description page of Project Settings.
//컴포넌트 안에 다시 컴포넌트(Instanced Static Mesh Component)를 만드는 과정이 복잡해서 그냥 로컬맵 안에 기능을 넣어버리고 컴포넌트는 쓰지않음.

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"

#include "Header/Struct.h"
#include "Header/Struct_Tile.h"
#include "Header/Struct_Renderer.h"

#include "HHM_Component_LocalMapRender.generated.h"

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API UHHM_Component_LocalMapRender : public UPrimitiveComponent
{
	GENERATED_BODY()
	
public:
	UHHM_Component_LocalMapRender();

private:
	UPROPERTY()
		FHHM_MapInfo									m_MapInfo;
	UPROPERTY()
		int32											m_MaxMapIndex = 0;

	UPROPERTY(VisibleAnywhere)
		class UStaticMesh*								m_pStaticMesh = nullptr;
	/*UPROPERTY()
		TArray<class UInstancedStaticMeshComponent*>	m_Container_Comp_InstancedMesh;*/
	UPROPERTY(VisibleAnywhere)
		TMap<int32, FHHM_InstancedMeshArray>			m_Container_Comp_InstancedMesh;

	UPROPERTY(VisibleAnywhere)
		class UInstancedStaticMeshComponent*			m_Temp_InstancedMeshComp = nullptr;

	UPROPERTY(VisibleAnywhere)
		TArray<FHHM_RenderData>							m_Container_RenderData;



public:
	UFUNCTION(BlueprintCallable)
		bool Set_MapData(const TArray<FHHM_TileData>& _arr_TileInfo, const FHHM_MapInfo& _mapInfo);
	
	UFUNCTION(BlueprintCallable)
		bool Set_TileRenderData(int32 _index, int32 _tileID, int32 _tileSubID, FTransform& _transform);
	UFUNCTION(BlueprintCallable)
		bool Update_TileRenderData(class ALocalMap* _pLocalMap, const FHHM_TileData& _tileData);

#pragma region Initialize
public:
	bool	Initialize_Renderer(void);
private:
	bool	Initialize_MeshComponent(void);
	bool	Initialize_Container_InstancedMesh(void);

	bool	Clear_MeshComponent(void);
	bool	Clear_RenderData(void);
#pragma endregion

private:
	bool	RenderInstance_Add(int32 _tileID, int32 _tileSubID, int32 _index_Tile, FTransform& _tileTransform);
	bool	RenderInstance_Remove(int32 _index_Tile);

	bool	Check_ValidRenderID(int32 _tileID, int32 _tileSubID);
	bool	Check_ValidIndex(int32 _index);

};
