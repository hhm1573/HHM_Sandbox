// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Data/Item/HHM_Item.h"
#include "Data/InventorySlotData/HHM_InventorySlotData_Row.h"

#include "HHM_Component_Inventory.generated.h"


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



protected:
	// �κ��丮 ����
	UPROPERTY()
		TArray<FHHM_InventorySlotData_Row>		m_InventorySlotData;
	// ������ ������
		TMap<int32, TSharedPtr<UHHM_Item>>		m_Container_Item;
	// �κ��丮 ������ ����
	UPROPERTY()
		int32									m_InventorySize_Horizontal = 0;
	UPROPERTY()
		int32									m_InventorySize_Vertical = 0;



public:
	// �ʱ�ȭ
	bool	Initialize_Inventory(int32 _size_Horizontal, int32 _size_Vertical);


	
	// ������ ���Ի��� �̵����� �������̽�
	bool	Insert_Item_At(int32 _index_Horizontal, int32 _index_Vertical, TSharedPtr<UHHM_Item>& _pItem);
	
	/*
	* Return - 1=General Error / 2=Inventory Full
	*/
	int32	Intert_Item(TSharedPtr<UHHM_Item>& _pItem);



	FIntPoint	Convert_IndexToIndexPoint(int32 _index);

	

protected:
	// ������ ���԰��� ���� üũ
	bool		Check_IsRoomFree(int32 _index_Horizontal, int32 _index_Vertical, int32 _size_Horizontal, int32 _size_Vertical);
	// ������ ���԰��� ���� ��û (������ -1)
	int32		Find_FreeRoom(int32 _size_Horizontal, int32 _size_Vertical);

	bool		Check_IsValidIndex(int32 _index_Horizontal, int32 _index_Vertical);

	int32		Find_ValidItemKey();
	bool		Set_RoomOccupied(int32 _index_Horizontal, int32 _index_Vertical, int32 _size_Horizontal, int32 _size_Vertical, int32 _itemKey);
	//bForceFree = true �ϰ�� �Էµ� ������ �κ��丮 ���� �����ϰų� ������ ������ Ű���� �ٸ���� ���� ���ܻ�Ȳ���� ���������� ������.
	void		Set_RoomFree(int32 _index_Horizontal, int32 _index_Vertical, int32 _size_Horizontal, int32 _size_Vertical, bool _bForceFree = false);
};
