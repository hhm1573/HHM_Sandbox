// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"

#include "Header/Struct_Tile.h"

#include "HHM_Entity.generated.h"



UCLASS()
class HHM_SANDBOX_API AHHM_Entity : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHHM_Entity();

protected:
	class ALocalMap*	m_pLocalMap = nullptr;
	int32				m_EntityID = -1;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginDestroy() override;



public:
	void Register_Entity(class ALocalMap* _pLocalMap, int32 _entityID);
	void DeRegister_Entity(void);
	const int32&	Get_ID(void) { return m_EntityID; }



public:
	UFUNCTION(BlueprintNativeEvent)
		void HHM_Jump(const FVector& jump_LocationStart, const FVector& jump_LocationEnd, const int32& jump_Height);
	virtual void HHM_Jump_Implementation(const FVector& jump_LocationStart, const FVector& jump_LocationEnd, const int32& jump_Height);

	UFUNCTION(BlueprintNativeEvent)
		void HHM_FallOff(const FVector& fall_LocationStart, const FVector& fall_LocationEnd, const int32& fall_Height);
	virtual void HHM_FallOff_Implementation(const FVector& fall_LocationStart, const FVector& fall_LocationEnd, const int32& fall_Height);

	UFUNCTION(BlueprintNativeEvent)
		void HHM_HorizontalJump(const FVector& _horizontalJump_LocationStart, const FVector& _horizontalJump_LocationEnd, const int32& _jump_Length);
	virtual void HHM_HorizontalJump_Implementation(const FVector& _horizontalJump_LocationStart, const FVector& _horizontalJump_LocationEnd, const int32 _jump_Length);



private:
	UFUNCTION(BlueprintCallable)
		bool Attack_Tile(int32 index_Horizontal, int32 index_Vertical, int32 damage);
	UFUNCTION(BlueprintCallable)	//Temporary function will be replaced with Use_Item or place_Item so that it can request is it placeable tile and if it is what is it's Tile info
		bool Place_Tile(int32 index_Horizontal, int32 index_Vertical, int32 tileID);

protected:
	void Request_LocalMap();

};
