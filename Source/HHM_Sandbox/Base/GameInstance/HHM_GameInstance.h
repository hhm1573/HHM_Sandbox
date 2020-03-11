// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Header/Struct.h"

#include "HHM_GameInstance.generated.h"

class AHHM_Manager;
class AHHM_Manager_Render;
//class AHHM_Manager_LocalMap;
class AHHM_Manager_Tile;
//class AHHM_Manager_Texture;

//class AHHM_Manager_Debug;

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API UHHM_GameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	UPROPERTY()
		FHHM_MapInfo				m_MapInfo;
	
	/*UPROPERTY()
		AHHM_Manager_Texture*		m_pManager_Texture = nullptr;*/

	UPROPERTY()
		AHHM_Manager_Render*		m_pManager_Render = nullptr;
	/*UPROPERTY()
		AHHM_Manager_LocalMap*		m_pManager_LocalMap = nullptr;*/
	UPROPERTY()
		AHHM_Manager_Tile*			m_pManager_Tile = nullptr;
	/*UPROPERTY()
		AHHM_Manager_Navigation*	m_pManager_Navigation = nullptr;*/
	
	/*UPROPERTY()
		AHHM_Manager_Debug*			m_pManager_Debug = nullptr;*/

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void		Initialize_Game();
	virtual void	Initialize_Game_Implementation();

public:
	UFUNCTION(BlueprintCallable)
		FHHM_MapInfo		Get_MapInfo() { return m_MapInfo; }

public:
	/*UFUNCTION(BlueprintCallable)
		AHHM_Manager_Render*		Get_Manager_Render() { return m_pManager_Render; }*/
	/*UFUNCTION(BlueprintCallable)
		AHHM_Manager_LocalMap*		Get_Manager_LocalMap() { return m_pManager_LocalMap; }*/
	/*UFUNCTION(BlueprintCallable)
		AHHM_Manager_Tile*			Get_Manager_Tile() { return m_pManager_Tile; }*/
	/*UFUNCTION(BlueprintCallable)
		AHHM_Manager_Debug*			Get_Manager_Debug() { return m_pManager_Debug; }*/
	/*UFUNCTION(BlueprintCallable)
		AHHM_Manager_Navigation*	Get_Manager_Navigation() { return m_pManager_Navigation; }*/
	
private:
	void Spawn_Manager(void);
	
};
//code order 1. Constructor thing / 2. Unreal function / 3.public-private variable / 4. public-private function