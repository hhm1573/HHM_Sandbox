// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HHM_ItemData.generated.h"

/**
 * 실제로 인벤토리간에 움직이는 아이템 데이터.
 * 데이터를 저장하는 용도로만 사용되고 실제 아이템의 기능을 수행하는 클래스는 따로 있다. (Data 가 안붙은 것)
 *
 * 인벤토리 등에서 아이템 데이터를 생성하거나 주고받을 때 포인터를 이용해서 관리하고, 기능을 수행하는 아이템 클래스에서 아이템 데이터를 사용할 때
 * 목적에 맞게 아이템 데이터에서 자식으로 확장된 클래스 (예: ItemData_Useable, ItemData_Weapon 등. 예시를 든것이지 확정된 자식형은 아님) 로 형변환 하여 사용하기 위해
 * 부모의 포인터로 자식을 보관할수 있게 포인터 사용을 위주로 하고, 이 과정에서 객체의 메모리관리등을 원활히 하고자 (Unreal 의 garbage collect 기능을 사용하고자)
 * 구조체가 아닌 UObject 자식형으로 구현하였다.
 */
UCLASS()
class HHM_SANDBOX_API UHHM_ItemData : public UObject
{
	GENERATED_BODY()
	//GENERATED_UCLASS_BODY()

public:
	UHHM_ItemData();



protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
		int32				m_ID = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
		int32				m_SubID = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
		class UHHM_Item*	m_Item = nullptr;

public:
	class UHHM_Item* Get_Item() { return m_Item; }

public:
	void Set_ID(int32 _id) { m_ID = _id; }
	void Set_SubID(int32 _subID) { m_SubID = _subID; }
	void Set_Item(class UHHM_Item* _pItem) { m_Item = _pItem; }

	
};
