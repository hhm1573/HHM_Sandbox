// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Base/Component/Inventory/HHM_InventoryItemData.h"
#include "Data/Item/ItemData/HHM_ItemData.h"
#include "Data/InventorySlotData/HHM_InventorySlotData_Row.h"

#include "HHM_Component_Inventory.generated.h"



// Note : FIntPoint 가 왠지 미완성된 구조체인듯한 찝찝한 기분이 들어 앵간하면 안쓰려고 했음.



//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHHM_Delegate_InventoryUpdate, const TMap<int32, FHHM_InventoryItemData>&, ItemContainer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHHM_Delegate_InventoryUpdate, int32, Container_ItemData);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HHM_SANDBOX_API UHHM_Component_Inventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHHM_Component_Inventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category = "HHM_Event")
		FHHM_Delegate_InventoryUpdate OnInventoryUpdate;		
	//Inventory UI 등에서 매 틱마다 인벤토리를 업데이트 하지 않고 Inventory Component가 직접 이벤트를 Broadcast 할 때 신호를 받고 업데이트 할 수 있게끔 해놓은 작업인듯 싶음.
	//아이템 슬롯마다 모두 업데이트를 호출하니 매 슬롯마다 아이템 그림을 지워야 할지 추가해야할지 알아보는 대신
	//슬롯 데이터에 Master / Source 여부를 저장하고 UI에서 해당 칸의 점유가 해제되었을때 기존에 아이템의 시작위치였으면 그림을 지우고 시작위치가 아니였으면 데이터만 업데이트 하는식으로 하면될듯.



protected:
	// 인벤토리 슬롯
	UPROPERTY()
		TArray<FHHM_InventorySlotData_Row>		m_InventorySlotData;
	// 아이템 데이터
	UPROPERTY()
		TMap<int32, FHHM_InventoryItemData>		m_Container_ItemData;
	// 인벤토리 사이즈 변수
	UPROPERTY()
		int32									m_InventorySize_Horizontal = 0;
	UPROPERTY()
		int32									m_InventorySize_Vertical = 0;



public:
	// 초기화
	bool	Initialize_Inventory(int32 _size_Horizontal, int32 _size_Vertical);

public:
	UHHM_ItemData*							Get_ItemDataPtr(int32 _index_Horizontal, int32 _index_Vertical);

	TMap<int32, FHHM_InventoryItemData>&	Get_ItemDataContainer_Ref() { return m_Container_ItemData; }

	FIntPoint								Get_InventorySize() { return FIntPoint(m_InventorySize_Horizontal, m_InventorySize_Vertical); }

public:
	bool	Check_IsValidIndex(int32 _index_Horizontal, int32 _index_Vertical);

	bool	Check_IsItemSwappable(int32 _index_Horizontal, int32 _index_Vertical, UHHM_ItemData*& _pItemData_Swap);

	bool	Check_IsItemInsertable(UHHM_ItemData*& _pItemData);

	bool	Check_IsItemInsertableAt(int32 _index_Horizontal, int32 _index_Vertical, UHHM_ItemData*& _pItemData_Insert);


	
public:
	// 아이템 삽입삭제 이동등의 인터페이스
	bool	Item_Insert_At(int32 _index_Horizontal, int32 _index_Vertical, UHHM_ItemData*& _pItemData);
	
	/*
	* Return - 1=General Error / 2=Inventory Full
	*/
	int32	Item_Insert(UHHM_ItemData*& _pItemData);

	/*
	* _pItemData_Return - nullptr is fine. no need to make new object to get return data
	*/
	bool	Item_Pop_At(UHHM_ItemData*& _pItemData_Return, int32 _index_Horizontal, int32 _index_Vertical);

	bool	Item_Remove(UHHM_ItemData*& _pItemData_Remove);



	FIntPoint	Convert_IndexToIndexPoint(int32 _index);

	int32		Convert_IndexPointToIndex(FIntPoint _indexPoint);

	

protected:
	// 아이템 삽입가능 공간 체크
	bool		Check_IsRoomFree(int32 _index_Horizontal, int32 _index_Vertical, int32 _size_Horizontal, int32 _size_Vertical);
	// 아이템 삽입가능 공간 요청 (없으면 -1)
	int32		Find_FreeRoom(int32 _size_Horizontal, int32 _size_Vertical);




	int32		Find_ValidItemKey();



	bool		Find_FirstSlot_ItemOccupied(FIntPoint& _indexPoint_Return, int32 _index_Item);



	bool		Set_RoomOccupied(int32 _index_Horizontal, int32 _index_Vertical, int32 _size_Horizontal, int32 _size_Vertical, int32 _itemKey, FHHM_InventoryItemData* _inventoryItemData);

	//bForceFree = true 일경우 입력된 공간이 인벤토리 밖을 포함하거나 슬롯의 아이템 키값이 다른경우 등의 예외상황에도 공간점유를 해제함.
	void		Set_RoomFree(int32 _index_Horizontal, int32 _index_Vertical, int32 _size_Horizontal, int32 _size_Vertical, bool _bForceFree = false);

	//search input indexpoint and if that slot is occupied, trace item. then free the room that item occupied and remove item from container.
	//void		CleanUp_At(int32 _index_Horizontal, int32 _index_Vertical);
};
