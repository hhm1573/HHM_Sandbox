// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Data/Item/RenderData/HHM_RenderData_Item.h"

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
UCLASS(Abstract)
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)	//On register, itemdata will get item class from Item Manager by this name.
		FString				m_ItemName;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
		const class UHHM_Item*	m_Item = nullptr;						//Note : 스태틱으로 변경가능

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
		int32				m_ItemSize_Horizontal = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
		int32				m_ItemSize_Vertical = 0;

	FHHM_RenderData_Item		m_RenderData;							//Note : 스태틱으로 하거나 따로 DefaultItemData 클래스같은걸 만들어서 하나만 존재하게 하거나 해야할듯?
	TArray<FString>				m_Container_Descriptions;





public:
	FString					Get_ItemName() const { return m_ItemName; }
	//class UHHM_Item* Get_Item() { return m_Item; }
	const class UHHM_Item* Get_Item() const { return m_Item; }
	//const class UHHM_Item* Get_Item_Const() const { return m_Item; }

	int32 Get_ID() const { return m_ID; }
	int32 Get_SubID() const { return m_SubID; }

	const FIntPoint Get_ItemSize() const { return FIntPoint(m_ItemSize_Horizontal, m_ItemSize_Vertical); }

public:
	void Set_ID(int32 _id) { m_ID = _id; }
	void Set_SubID(int32 _subID) { m_SubID = _subID; }
	void Set_Item(const class UHHM_Item* _pItem) { m_Item = _pItem; }

	

public:
	//Set RenderData for itemdata itself
	virtual void	Initialize_RenderData();

public:
	UFUNCTION(BlueprintCallable)	//HHM Note : Temporary Function. 타일엔티티와의 상호작용 테스트에 필요한 인벤토리에서 아이템 사진을 가져오는 기능을 위해 임시로 만든 함수.
		void		BP_Get_RenderData(class UMaterialInterface*& _material_Return, class ALocalMap* _pLocalMap);
public:
	virtual void	Get_RenderData_ItemActor(class UMaterialInterface*& _material_Return, FVector2D& _cubeSize_Return, class ALocalMap* _pLocalMap);

};
