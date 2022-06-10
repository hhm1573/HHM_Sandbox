// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"

#include "Header/Struct_Tile.h"

#include "Data/Interaction/HHM_Data_Action.h"

#include "Base/Component/Inventory/HHM_Component_Inventory.h"
#include "Base/Component/InteractionHandler/HHM_Component_InteractionHandler.h"

#include "Act/HHM_Act.h"

#include "HHM_Entity.generated.h"



UCLASS()
class HHM_SANDBOX_API AHHM_Entity : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHHM_Entity();

protected:
	UPROPERTY(BlueprintReadOnly, Category = HHM_Entity)
		class ALocalMap*	m_pLocalMap = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = HHM_Entity)
		int32				m_EntityID = -1;

	UPROPERTY()
		UHHM_Component_Inventory*			m_pComponent_Inventory = nullptr;
	UPROPERTY()
		UHHM_Component_InteractionHandler*	m_pComponent_InteractionHandler = nullptr;



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
	UHHM_Component_Inventory* Get_Component_Inventory() { return m_pComponent_Inventory; }



public:
	UFUNCTION(BlueprintCallable/*, meta=(AdvancedDisplay="_isRoot")*/)
		const TMap<int32, FHHM_Inventory_Grid>& Get_InventoryContainer(bool _isRoot);
	UFUNCTION(BlueprintCallable)
		bool Get_InventorySize(FIntPoint& _size_Return, int32 _inventoryID);



private:
	bool Register_Entity(void);
	void DeRegister_Entity(void);

public:
	class ALocalMap*		Get_LocalMap(void) { return m_pLocalMap; }
	const class ALocalMap*	Get_LocalMap_Const(void) { return m_pLocalMap; }
	const int32&			Get_ID(void) { return m_EntityID; }



public:
	// HHM To Do : Get_Order_Selected, Get_Order_Targeted (��ƼƼ ������ Ư�� ��ġ ��Ŭ���� ��Ÿ���� �޴� ������ ���� �Լ�)
	//TArray<MenuData> Get_OrderList_Selected(void) ���̳� ��ƼƼ���� ���۷��� Ÿ���� ��� ���� ������ ���. �̵� ����?
	//TArray<MenuData> Get_OrderList_Targeted(const AHHM_Entity& _entity_Selected) Selected ��ƼƼ�� ������ ���� ���� ��ƼƼ�� ��� ������ ���. ����, ��ȣ�ۿ� ��
	//MenuData ����ü�� ���ؼ��� �۾��� �ϸ� �����س����� ������ �ϴ� �⺻������ ��(����� UI�� ǥ�õ� ����), ���ID (Order_Handler�� ����ƼƼ��� ���� ���̵� ���� �ൿ,
	//�ϴ� ������ Ŭ�������� ���� ����� �����ϱ⿡ ��ɾ��̵� ��ġ�� ��쿡 ���� ����ؾ��� ���� ��������� Ȥ�� �𸣴� ��ɸ�ũ��?������Ʈ? ������ ����� ��ɸ����� �����ũ�θ� ��������
	//�۾��� �ϵ��� ����. �� Selected Entity�� Targeted Entity, �� ��ƼƼ�� ����� �������� ����� Selected TileEntity, Targeted TileEntity �ϴ� �������� �־ �ɵ�.



#pragma region Act

public:
	//Movement component lock ��, ��ȣ�ۿ��� ���۵ǰų� ����ɶ� �ڽ��� ���¸� ��ȭ�ϱ����� �Լ�.
	virtual void Interaction_Begin();
	virtual void Interaction_End();

#pragma endregion



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
		bool Place_Tile(int32 index_Horizontal, int32 index_Vertical, int32 tileID, int32 tileSubID);

	//Protected Use_Item. Use item and consume item
};
