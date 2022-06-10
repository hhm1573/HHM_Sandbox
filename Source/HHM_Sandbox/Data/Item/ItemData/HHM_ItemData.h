// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Data/Item/RenderData/HHM_RenderData_Item.h"
#include "Engine/Texture.h"

#include "HHM_ItemData.generated.h"

/**
 * ������ �κ��丮���� �����̴� ������ ������.
 * �����͸� �����ϴ� �뵵�θ� ���ǰ� ���� �������� ����� �����ϴ� Ŭ������ ���� �ִ�. (Data �� �Ⱥ��� ��)
 *
 * �κ��丮 ��� ������ �����͸� �����ϰų� �ְ���� �� �����͸� �̿��ؼ� �����ϰ�, ����� �����ϴ� ������ Ŭ�������� ������ �����͸� ����� ��
 * ������ �°� ������ �����Ϳ��� �ڽ����� Ȯ��� Ŭ���� (��: ItemData_Useable, ItemData_Weapon ��. ���ø� ������� Ȯ���� �ڽ����� �ƴ�) �� ����ȯ �Ͽ� ����ϱ� ����
 * �θ��� �����ͷ� �ڽ��� �����Ҽ� �ְ� ������ ����� ���ַ� �ϰ�, �� �������� ��ü�� �޸𸮰������� ��Ȱ�� �ϰ��� (Unreal �� garbage collect ����� ����ϰ���)
 * ����ü�� �ƴ� UObject �ڽ������� �����Ͽ���.
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
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
		const class UHHM_Item*	m_Item = nullptr;						//Note : ����ƽ���� ���氡��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
		int32				m_ItemSize_Horizontal = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemData)
		int32				m_ItemSize_Vertical = 0;

	FHHM_RenderData_Item		m_RenderData;							//Note : ����ƽ���� �ϰų� ���� DefaultItemData Ŭ���������� ���� �ϳ��� �����ϰ� �ϰų� �ؾ��ҵ�?
	UPROPERTY()	//UI�󿡼��� ǥ�ø� ���� �ؽ�ó�� �����ϴ� ����
	TArray<UTexture2D*>			m_Container_Texture;
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
	UFUNCTION(BlueprintCallable)	//HHM Note : Temporary Function. Ÿ�Ͽ�ƼƼ���� ��ȣ�ۿ� �׽�Ʈ�� �ʿ��� �κ��丮���� ������ ������ �������� ����� ���� �ӽ÷� ���� �Լ�.
		void		BP_Get_RenderData(class UMaterialInterface*& _material_Return, class ALocalMap* _pLocalMap);
	UFUNCTION(BlueprintCallable)
		void		BP_Get_Texture2D(UTexture2D*& _pTexture_Return);
public:
	virtual void	Get_RenderData_ItemActor(class UMaterialInterface*& _material_Return, FVector2D& _cubeSize_Return, class ALocalMap* _pLocalMap);
	//Inventory UI�� UI���� ���� �ؽ��ĸ� ��ȯ�޴� �Լ�. UI���� �ؽ��ĸ� ������Ʈ�Ҷ� ȣ��ȴ�.
	virtual void	Get_Texture(UTexture2D*& _pTexture_Return);

};
