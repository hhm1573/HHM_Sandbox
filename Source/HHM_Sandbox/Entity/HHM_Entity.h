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
	// HHM To Do : Get_Order_Selected, Get_Order_Targeted (엔티티 선택후 특정 위치 우클릭시 나타나는 메뉴 생성을 위한 함수)
	//TArray<MenuData> Get_OrderList_Selected(void) 블럭이나 엔티티등의 레퍼런스 타겟이 없어도 실행 가능한 명령. 이동 같은?
	//TArray<MenuData> Get_OrderList_Targeted(const AHHM_Entity& _entity_Selected) Selected 엔티티의 정보를 토대로 현재 엔티티에 사용 가능한 명령. 공격, 상호작용 등
	//MenuData 구조체에 대해서는 작업을 하며 수정해나가야 겠지만 일단 기본적으로 라벨(명령이 UI에 표시될 문구), 명령ID (Order_Handler나 블럭엔티티등에서 받은 아이디를 토대로 행동,
	//일단 각각의 클래스에서 따로 명령을 수행하기에 명령아이디가 곂치는 경우에 대해 고민해야할 일은 없어보이지만 혹시 모르니 명령매크로?컨스턴트? 파일을 만들어 명령마다의 상수매크로를 만들어놓고
	//작업을 하도록 하자. 또 Selected Entity와 Targeted Entity, 블럭 엔티티도 명령을 받을것을 고려해 Selected TileEntity, Targeted TileEntity 일단 이정도만 있어도 될듯.



#pragma region Act

public:
	//Movement component lock 등, 상호작용이 시작되거나 종료될때 자신의 상태를 변화하기위한 함수.
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
