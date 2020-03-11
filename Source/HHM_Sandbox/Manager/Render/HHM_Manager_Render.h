// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/HHM_Manager.h"

#include "Header/Struct.h"
#include "Header/Struct_Tile.h"
#include "Header/Struct_Renderer.h"

#include "HHM_Manager_Render.generated.h"

class UInstancedStaticMeshComponent;
class ALocalMap;

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API AHHM_Manager_Render : public AHHM_Manager
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
		FHHM_MapInfo		m_MapInfo;
	UPROPERTY()
		int32				m_MaxMapIndex = 0;

	UPROPERTY()
		class UStaticMesh*	m_pStaticMesh_Plane = nullptr;
	UPROPERTY(VisibleAnywhere)
		TArray<UInstancedStaticMeshComponent*> m_Arr_pInstancedMesh;

	UPROPERTY()
		TMap<int32, FHHM_InstancedMeshArray>		m_Map_Instanced;

	UPROPERTY()
		TArray<FHHM_RenderData>		m_Arr_RenderInfo;

public:
	void Update_MapInfo(void);

	UFUNCTION(BlueprintCallable)
		void Set_RenderMapData(const TArray<FHHM_TileData>& Arr_TileInfo);
	UFUNCTION(BlueprintCallable)
		void Set_TileRenderData(int32 index, int32 tileID, int32 tileSubID, FTransform transform);
	/* 
	Basically "Set_TileRenderData" and "Update_RenderInfo_Tile" do the same job.
	both function get information for rendering(ID,SubID,Transform) than remove old instance and replace new one.
	but "Set_TileRenderData" doesn't call tile's 'render update' function.
	so "Update_RenderInfo_Tile" is bit more expensive but bit much easier to use cause it only need tile info struct which store at map data.
	long story short, "Set_TileRenderData" is called by tile it self which who doesn't need to call 'render update' function to get information.
	and "Update_RenderInfo_Tile" is called by render manager it self or others who need to call 'render update' function.
	"Set_TileRenderData"�� "Update_RenderInfo_Tile"�� �Ѵ� ���������� �����ϴ� ����� �ϴ� �Լ�������, ���ڴ� Ÿ���� ID,SubID,transform ���� ���� �Է¹޾� �ٷ� ����� �����ϰ�,
	���ڴ� Ÿ���� ���������� �����ϴ� �Լ��� ȣ���Ͽ� ������ �޾ƿ��� �� ������ ���� ���������� �����մϴ�.
	������ ��� Ÿ�� Ŭ����,ID,SubID �� ���� ���������� �̹� �˰��ִ� ��쿡 ȣ��ǰ�, �׿��� ��� (���������� �޾ƿ��� �Լ��� ȣ���ؾ� �ϴ� ���) ������ �Լ��� �̿��մϴ�.
	*/
	void	Update_RenderInfo_Tile(class ALocalMap* pLocalMap, FHHM_TileData& tileInfo);

private:
	void	Initialize_MeshComponent(void);
	void	Initialize_Storage_InstancedMesh(void);

	void	Clear_MeshComponent(void);
	void	Clear_RenderData(void);

	//void	RenderInstance_Add(int32 tileID, int32 tileSubID, int32 index_Tile);
	bool	RenderInstance_Add(int32 tileID, int32 tileSubID, int32 index_Tile, FTransform& tileTransform);
	bool	RenderInstance_Remove(int32 index_Tile);

	//Check is Tile ID is valid for render. Return true when is valid.
	bool	Check_ValidTileID(int32 tileID, int32 tileSubID);
	bool	Check_ValidIndex(int32 index);
};
