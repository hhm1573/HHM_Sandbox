// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Data/Item/ItemData/HHM_ItemData.h"
#include "Data/Inventory/Base/Item/HHM_Data_Inventory_Item.h"
#include "Data/Inventory/Base/InventorySlot/HHM_Data_Inventory_Slot.h"

#include "Data/Inventory/HHM_Inventory.h"

#include "HHM_Component_Inventory.generated.h"



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
	//Inventory UI ��� �� ƽ���� �κ��丮�� ������Ʈ ���� �ʰ� Inventory Component�� ���� �̺�Ʈ�� Broadcast �� �� ��ȣ�� �ް� ������Ʈ �� �� �ְԲ� �س��� �۾��ε� ����.
	//������ ���Ը��� ��� ������Ʈ�� ȣ���ϴ� �� ���Ը��� ������ �׸��� ������ ���� �߰��ؾ����� �˾ƺ��� ���
	//���� �����Ϳ� Master / Source ���θ� �����ϰ� UI���� �ش� ĭ�� ������ �����Ǿ����� ������ �������� ������ġ������ �׸��� ����� ������ġ�� �ƴϿ����� �����͸� ������Ʈ �ϴ½����� �ϸ�ɵ�.



protected:
	//// �κ��丮 ����
	//UPROPERTY()
	//	TArray<FHHM_InventorySlotData_Row>		m_InventorySlotData;
	//// ������ ������
	//UPROPERTY()
	//	TMap<int32, FHHM_InventoryItemData>		m_Container_ItemData;
	//// �κ��丮 ������ ����
	//UPROPERTY()
	//	int32									m_InventorySize_Horizontal = 0;
	//UPROPERTY()
	//	int32									m_InventorySize_Vertical = 0;
	UPROPERTY()
		TMap<int32, FHHM_Inventory>				m_Container_Inventory_Root;
	UPROPERTY()
		TMap<int32, FHHM_Inventory>				m_Container_Inventory;



public:
	int32		Inventory_Add(const bool& _isRoot, const FHHM_Data_Inventory& _data_Inventory);
	


public:
	UHHM_ItemData*							Get_ItemDataPtr(const bool& _isRoot, const int32& _inventoryID, const int32& _index_Horizontal, const int32& _index_Vertical);

	//TMap<int32, FHHM_Data_Inventory_Item>&	Get_ItemDataContainer_Ref(const bool& _isRoot, const int32& _inventoryID);

	bool									Get_InventorySize(FIntPoint& _inventorySize_Return, const bool& _isRoot, const int32& _inventoryID);



public:
	bool	Check_IsValidIndex(const bool& _isRoot, const int32& _inventoryID, const int32& _index_Horizontal, const int32& _index_Vertical);

	bool	Check_IsValidInventoryID(const bool& _isRoot, const int32& _inventoryID);

	bool	Check_IsItemSwappable(const bool& _isRoot, const int32& _inventoryID, const int32& _index_Horizontal, const int32& _index_Vertical, const UHHM_ItemData*& _pItemData_Swap);

	bool	Check_IsItemInsertable(const bool& _isRoot, const int32& _inventoryID, const UHHM_ItemData*& _pItemData_Insert);

	bool	Check_IsItemInsertableAt(const bool& _isRoot, const int32& _inventoryID, const int32& _index_Horizontal, const int32& _index_Vertical, const UHHM_ItemData*& _pItemData_Insert);


	
public:
	// ������ ���Ի��� �̵����� �������̽�


	//Insert item at target inventory's target slot
	EHHM_InventoryReturn	Item_Insert_At(int32& _inventoryItemID_Return, UHHM_ItemData*& _pItemData_Insert, const bool& _isRoot, const int32& _inventoryID, const int32& _index_Horizontal, const int32& _index_Vertical);
	
	//Insert item at target inventory
	EHHM_InventoryReturn	Item_Insert_AtInventory(int32& _inventoryItemID_Return, UHHM_ItemData*& _pItemData_Insert, const bool& _isRoot, const int32& _inventoryID);

	//Insert item at any inventory
	EHHM_InventoryReturn	Item_Insert(int32& _inventoryItemID_Return, int32& _inventoryID_Return, UHHM_ItemData*& _pItemData_Insert, const bool& _isRoot);



	EHHM_InventoryReturn	Item_Pop_At(UHHM_ItemData*& _pItemData_Return, const bool& _isRoot, const int32& _inventoryID, const int32& _index_Horizontal, const int32& _index_Vertical);

	//EHHM_InventoryReturn	Item_Remove(UHHM_ItemData*& _pItemData_Remove, const bool& _isRoot, const int32& _inventoryID);



	/*FIntPoint	Convert_IndexToIndexPoint(int32 _index);

	int32		Convert_IndexPointToIndex(FIntPoint _indexPoint);*/

	

protected:
	int32					Get_AvailableInventoryID(const bool& _isRoot);

	// ������ ���԰��� ���� üũ
	bool					Check_IsRoomFree(const bool& _isRoot, const int32& _inventoryID, const int32& _index_Horizontal, const int32& _index_Vertical, const int32& _size_Horizontal, const int32& _size_Vertical);
	//// Find available slot for item at target inventory
	//EHHM_InventoryReturn	Find_FreeRoom_AtInventory(int32& _index_Return, const bool& _isRoot, const int32& _inventoryID, const int32& _size_Horizontal, const int32& _size_Vertical);
	//// Find available slot for item at any inventory
	//EHHM_InventoryReturn	Find_FreeRoom(int32& _index_Return, bool& _isRoot_Return, int32& _inventoryID_Return, const int32& _size_Horizontal, const int32& _size_Vertical);
	




	//int32		Find_ValidItemKey();



	//bool		Find_FirstSlot_ItemOccupied(FIntPoint& _indexPoint_Return, int32 _index_Item);



	//bool		Set_RoomOccupied(int32 _index_Horizontal, int32 _index_Vertical, int32 _size_Horizontal, int32 _size_Vertical, int32 _itemKey, FHHM_InventoryItemData* _inventoryItemData);

	////bForceFree = true �ϰ�� �Էµ� ������ �κ��丮 ���� �����ϰų� ������ ������ Ű���� �ٸ���� ���� ���ܻ�Ȳ���� ���������� ������.
	//void		Set_RoomFree(int32 _index_Horizontal, int32 _index_Vertical, int32 _size_Horizontal, int32 _size_Vertical, bool _bForceFree = false);

	////search input indexpoint and if that slot is occupied, trace item. then free the room that item occupied and remove item from container.
	////void		CleanUp_At(int32 _index_Horizontal, int32 _index_Vertical);
};
