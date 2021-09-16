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
		TMap<int32, FHHM_Data_Inventory_Item>			m_Container_ItemData;	// 다른 Inventory나 UI등에서 아이템의 ID를 이용해서 참고할 경우를 대비해 Array가 아닌 Map으로 저장한다.

	

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
	//Inventory Component 와 Inventory 를 분리하기 이전 Inventory Component에 구현되어있던 Inventory는 IndexPoint 대신 Index를 저장하여 아이템을 관리하였지만,
	//분리하는 과정에서 혹시 모르니 그냥 IndexPoint를 사용하게끔 변경하여 아래 Convert 함수는 사실상 쓸일이 없어졌다.
	//하지만 무한인벤토리와 같이 혹시나 가로사이즈가 변경될 수 있
	FIntPoint		Convert_IndexToIndexPoint(const int32& _index) const;
	int32			Convert_IndexPointToIndex(const FIntPoint& _indexPoint) const;



public:
	bool		Check_IsRoomFree(const int32& _index_Horizontal, const int32& _index_Vertical, const int32& _size_Horizontal, const int32& _size_Vertical) const;
	
	//Note : 나중에 에러가 나서 어디서 에러가 난건지 알아내야 한다거나 하는 이유로 에러 유무를 반환해야 하게 되면 
	//리턴값을 EHHM_InventoryReturn으로 바꾸고 리턴값을 인자로 받은 레퍼런스나 포인터로 반환하게끔 설정
	int32		Find_FreeRoom(const int32& _size_Horizontal, const int32& _size_Vertical) const;

protected:
	int32		Find_ValidInventoryItemID() const;

	EHHM_InventoryReturn	Find_SourceIndex(FIntPoint& _indexPoint_Return, const int32 _index_InventoryItemID);

	

protected:
	bool		Set_RoomOccupied(const int32& _index_Horizontal, const int32& _index_Vertical, const int32& _size_Horizontal, const int32& _size_Vertical
									, const int32& _inventoryItemID, FHHM_Data_Inventory_Item* _pInventoryItemData);

	void		Set_RoomFree(const int32& _inventoryItemID);

	//bForceFree = true 일경우 입력된 공간이 인벤토리 밖을 포함하거나 슬롯의 아이템 키값이 다른경우 등의 예외상황에도 공간점유를 해제함.
	void		Set_RoomFree_At(const int32& _index_Horizontal, const int32& _index_Vertical, const int32& _size_Horizontal, const int32& _size_Vertical, const bool& _bForceFree = false);


};