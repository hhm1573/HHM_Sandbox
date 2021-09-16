#pragma once

#include "CoreMinimal.h"

#include "Base/HHM_Data_Inventory.h"
#include "Base/Item/HHM_Data_Inventory_Item.h"
#include "Base/InventorySlot/HHM_Data_Inventory_Slot_Row.h"
#include "Base/Enum_InventoryReturn.h"

#include "ObjectMacros.h"



USTRUCT()
struct FHHM_Inventory
{
	GENERATED_USTRUCT_BODY()

public:
	FHHM_Inventory();

protected:
	UPROPERTY()	//Collective of datas about inventory.
		FHHM_Data_Inventory								m_Data_Inventory;

	UPROPERTY()
		TArray<FHHM_Data_Inventory_Slot_Row>	m_InventorySlotData;	//Column of Slots
	UPROPERTY()
		TMap<int32, FHHM_Data_Inventory_Item>			m_Container_ItemData;	// �ٸ� Inventory�� UI��� �������� ID�� �̿��ؼ� ������ ��츦 ����� Array�� �ƴ� Map���� �����Ѵ�.

	

public:
	bool	Initialize_Inventory(const FHHM_Data_Inventory& _data_Inventory);

public:
	FIntPoint									Get_InventorySize();

	/**
	* @param	_pItemData_Return	Item Data on target slot
	* @return
	* Return "NoRoom" if input index is invalid.
	* Return "NoItem" if there is no item on target slot.
	**/
	EHHM_InventoryReturn						Get_ItemDataPtr_AtSlot(UHHM_ItemData*& _pItemData_Return, uint32 _index_Horizontal, uint32 _index_Vertical);

	TMap<int32, FHHM_Data_Inventory_Item>&		Get_ItemDataContainer_Ref();



public:
	bool Check_IsValidIndex(const uint32& _index_Horizontal, const uint32& _index_Vertical) const;
	
	bool Check_IsItemSwappable(const int32& _index_Horizontal, const int32& _index_Vertical, const UHHM_ItemData*& _pItemData_Swap) const;

	//Insert Item Wherever it can fit when there is enough room for item on inventory
	bool Check_IsItemInsertable(UHHM_ItemData*& _pItemData) const;

	bool Check_IsItemInsertable_At(const int32& _index_Horizontal, const int32& _index_Vertical, UHHM_ItemData*& _pItemData_Insert) const;



public:
	/**
	 * If inventory has enough room for item, insert it anywhere closest from start (left top)
	 *
	 * @param _return_ItemID		Item ID On Inventory
	 * @param _pItemData			Item Data To Insert
	 * @return						Result of item insertion
	 * @see DivideAndRoundDown
	 */
	EHHM_InventoryReturn	Item_Insert(int32& _InventoryItemID_Return, UHHM_ItemData*& _pItemData);

	EHHM_InventoryReturn	Item_Insert_At(int32& _InventoryItemID_Return, UHHM_ItemData*& _pItemData, const int32& _index_Horizontal, const int32& _index_Vertical);



	EHHM_InventoryReturn	Item_Pop_At(UHHM_ItemData*& _pItemData_Return, const int32& _index_Horizontal, const int32& _index_Vertical);

	EHHM_InventoryReturn	Item_Remove(UHHM_ItemData*& _pItemData_Remove);



	void					Clear_Inventory();



public:	
	//Inventory Component �� Inventory �� �и��ϱ� ���� Inventory Component�� �����Ǿ��ִ� Inventory�� IndexPoint ��� Index�� �����Ͽ� �������� �����Ͽ�����,
	//�и��ϴ� �������� Ȥ�� �𸣴� �׳� IndexPoint�� ����ϰԲ� �����Ͽ� �Ʒ� Convert �Լ��� ��ǻ� ������ ��������.
	//������ �����κ��丮�� ���� Ȥ�ó� ���λ���� ����� �� ��
	FIntPoint		Convert_IndexToIndexPoint(const int32& _index) const;
	int32			Convert_IndexPointToIndex(const FIntPoint& _indexPoint) const;



public:
	bool		Check_IsRoomFree(const int32& _index_Horizontal, const int32& _index_Vertical, const int32& _size_Horizontal, const int32& _size_Vertical) const;
	
	//Note : ���߿� ������ ���� ��� ������ ������ �˾Ƴ��� �Ѵٰų� �ϴ� ������ ���� ������ ��ȯ�ؾ� �ϰ� �Ǹ� 
	//���ϰ��� EHHM_InventoryReturn���� �ٲٰ� ���ϰ��� ���ڷ� ���� ���۷����� �����ͷ� ��ȯ�ϰԲ� ����
	int32		Find_FreeRoom(const int32& _size_Horizontal, const int32& _size_Vertical) const;

protected:
	int32		Find_ValidInventoryItemID() const;

	EHHM_InventoryReturn	Find_SourceIndex(FIntPoint& _indexPoint_Return, const int32 _index_InventoryItemID);

	

protected:
	bool		Set_RoomOccupied(const int32& _index_Horizontal, const int32& _index_Vertical, const int32& _size_Horizontal, const int32& _size_Vertical
									, const int32& _inventoryItemID, FHHM_Data_Inventory_Item* _pInventoryItemData);

	void		Set_RoomFree(const int32& _inventoryItemID);

	//bForceFree = true �ϰ�� �Էµ� ������ �κ��丮 ���� �����ϰų� ������ ������ Ű���� �ٸ���� ���� ���ܻ�Ȳ���� ���������� ������.
	void		Set_RoomFree_At(const int32& _index_Horizontal, const int32& _index_Vertical, const int32& _size_Horizontal, const int32& _size_Vertical, const bool& _bForceFree = false);


};