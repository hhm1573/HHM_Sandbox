// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Base/Component/Inventory/HHM_InventoryItemData.h"
#include "Data/Item/ItemData/HHM_ItemData.h"
#include "Data/InventorySlotData/HHM_InventorySlotData_Row.h"

#include "HHM_Component_Inventory.generated.h"



// Note : FIntPoint �� ���� �̿ϼ��� ����ü�ε��� ������ ����� ��� �ް��ϸ� �Ⱦ����� ����.



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
	// �κ��丮 ����
	UPROPERTY()
		TArray<FHHM_InventorySlotData_Row>		m_InventorySlotData;
	// ������ ������
	UPROPERTY()
		TMap<int32, FHHM_InventoryItemData>		m_Container_ItemData;
	// �κ��丮 ������ ����
	UPROPERTY()
		int32									m_InventorySize_Horizontal = 0;
	UPROPERTY()
		int32									m_InventorySize_Vertical = 0;



public:
	// �ʱ�ȭ
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
	// ������ ���Ի��� �̵����� �������̽�
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
	// ������ ���԰��� ���� üũ
	bool		Check_IsRoomFree(int32 _index_Horizontal, int32 _index_Vertical, int32 _size_Horizontal, int32 _size_Vertical);
	// ������ ���԰��� ���� ��û (������ -1)
	int32		Find_FreeRoom(int32 _size_Horizontal, int32 _size_Vertical);




	int32		Find_ValidItemKey();



	bool		Find_FirstSlot_ItemOccupied(FIntPoint& _indexPoint_Return, int32 _index_Item);



	bool		Set_RoomOccupied(int32 _index_Horizontal, int32 _index_Vertical, int32 _size_Horizontal, int32 _size_Vertical, int32 _itemKey, FHHM_InventoryItemData* _inventoryItemData);

	//bForceFree = true �ϰ�� �Էµ� ������ �κ��丮 ���� �����ϰų� ������ ������ Ű���� �ٸ���� ���� ���ܻ�Ȳ���� ���������� ������.
	void		Set_RoomFree(int32 _index_Horizontal, int32 _index_Vertical, int32 _size_Horizontal, int32 _size_Vertical, bool _bForceFree = false);

	//search input indexpoint and if that slot is occupied, trace item. then free the room that item occupied and remove item from container.
	//void		CleanUp_At(int32 _index_Horizontal, int32 _index_Vertical);
};
