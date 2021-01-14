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
	// 인벤토리 슬롯
	UPROPERTY()
		TArray<FHHM_InventorySlotData_Row>		m_InventorySlotData;
	// 아이템 데이터
		TMap<int32, TSharedPtr<UHHM_Item>>		m_Container_Item;
	// 인벤토리 사이즈 변수
	UPROPERTY()
		int32									m_InventorySize_Horizontal = 0;
	UPROPERTY()
		int32									m_InventorySize_Vertical = 0;



public:
	// 초기화
	bool	Initialize_Inventory(int32 _size_Horizontal, int32 _size_Vertical);


	
	// 아이템 삽입삭제 이동등의 인터페이스
	bool	Insert_Item_At(int32 _index_Horizontal, int32 _index_Vertical, TSharedPtr<UHHM_Item>& _pItem);
	
	/*
	* Return - 1=General Error / 2=Inventory Full
	*/
	int32	Intert_Item(TSharedPtr<UHHM_Item>& _pItem);



	FIntPoint	Convert_IndexToIndexPoint(int32 _index);

	

protected:
	// 아이템 삽입가능 공간 체크
	bool		Check_IsRoomFree(int32 _index_Horizontal, int32 _index_Vertical, int32 _size_Horizontal, int32 _size_Vertical);
	// 아이템 삽입가능 공간 요청 (없으면 -1)
	int32		Find_FreeRoom(int32 _size_Horizontal, int32 _size_Vertical);

	bool		Check_IsValidIndex(int32 _index_Horizontal, int32 _index_Vertical);

	int32		Find_ValidItemKey();
	bool		Set_RoomOccupied(int32 _index_Horizontal, int32 _index_Vertical, int32 _size_Horizontal, int32 _size_Vertical, int32 _itemKey);
	//bForceFree = true 일경우 입력된 공간이 인벤토리 밖을 포함하거나 슬롯의 아이템 키값이 다른경우 등의 예외상황에도 공간점유를 해제함.
	void		Set_RoomFree(int32 _index_Horizontal, int32 _index_Vertical, int32 _size_Horizontal, int32 _size_Vertical, bool _bForceFree = false);
};
