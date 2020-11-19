// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Header/Struct.h"

#include "HHM_GameMode_LocalMap.generated.h"

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API AHHM_GameMode_LocalMap : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AHHM_GameMode_LocalMap();

protected:
	UPROPERTY(VisibleAnywhere)
		class UHHM_Provider_Entity*		m_pProvider_Entity = nullptr;

	UPROPERTY(VisibleAnywhere)
		class AHHM_Manager_Tile*		m_pManager_Tile = nullptr;
	UPROPERTY(VisibleAnywhere)
		class AHHM_Manager_Navigation*	m_pManager_Navigation = nullptr;
	UPROPERTY(VisibleAnywhere)
		class AHHM_Manager_LocalMap*	m_pManager_LocalMap = nullptr;

protected:
	UPROPERTY(VisibleAnywhere)
		FHHM_MapInfo				m_MapInfo;

protected:



public:
	virtual void	BeginPlay() override;

#pragma region Utilities

//public:
//	bool			Utility_Index_Seperate(FVector2D& _vec_Return, int32 _index);
//	bool			Utility_Index_Combine(int32& _index_Return, FVector2D _vec_Index);
//	bool			Utility_Calculate_IndexLocation(FVector2D& _vec_Return, float _location_X, float _location_Z);
//	bool			Utility_Calculate_Location(FVector& _vec_Return, FVector2D _vec_IndexLocation, float _location_Y = 0.0f);

#pragma endregion



public:
	UFUNCTION(BlueprintCallable)
		class UHHM_Provider_Entity*				Get_Provider_Entity() { return m_pProvider_Entity; }
	UFUNCTION(BlueprintCallable)
		class AHHM_Manager_Tile*				Get_Manager_Tile() { return m_pManager_Tile; }
		const class AHHM_Manager_Tile*			Get_Manager_Tile_Const() const { return m_pManager_Tile; }
	UFUNCTION(BlueprintCallable)
		class AHHM_Manager_LocalMap*			Get_Manager_LocalMap() { return m_pManager_LocalMap; }
		const class AHHM_Manager_LocalMap*		Get_Manager_LocalMap_Const() const { return m_pManager_LocalMap; }
	UFUNCTION(BlueprintCallable)
		class AHHM_Manager_Navigation*			Get_Manager_Navigation() { return m_pManager_Navigation; }
		const class AHHM_Manager_Navigation*	Get_Manager_Navigation_Const() const { return m_pManager_Navigation; }
	UFUNCTION(BlueprintCallable)
		const FHHM_MapInfo&						Get_MapInfo() { return m_MapInfo; }


#pragma region Entity Handling

public:
	class AHHM_Entity* Spawn_Entity_By_ID(int32 _id_Entity, int32 _id_Map_Horizontal, int32 _id_Map_Vertical, int32 _index_Horizontal, int32 _index_Vertical, float _offset_Depth);
	class AHHM_Entity* Spawn_Entity_By_ID(int32 _id_Entity, int32 _id_Map, int32 _index, float _offset_Depth);
	UFUNCTION(BlueprintCallable)
		class AHHM_Entity* Spawn_Entity_By_ID(int32 _id_Entity, int32 _id_Map, int32 _index_Horizontal, int32 _index_Vertical, float _offset_Depth);

	bool				Register_Entity(class AHHM_Entity* _pEntity, int32* _pEntityID_Return, class ALocalMap** _ppLocalMap_Return, const FVector& _location);	//Put address of entity's member variables as input
	int32				Get_LocalMap_Index_ByLocation(const FVector& _location);
	class ALocalMap*	Get_LocalMap_ByLocation(const FVector& _location);
	class ALocalMap*	Get_LocalMap_ByIndex(const int32& _index);

#pragma endregion



#pragma region Valid Check

private:
	//return true if Index is valid.
	bool	Check_IsValidIndex(int32 _index);
	bool	Check_IsValidIndex(int32 _index_Horizontal, int32 _index_Vertical);

#pragma endregion
	
};
