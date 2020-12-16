// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "Header/Enum_PlayerController.h"

#include "HHM_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API AHHM_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHHM_PlayerController();



private:
	UPROPERTY()
		EHHM_ControllerMode m_eControllerMode = EHHM_ControllerMode::ControllerMode_Selection;

	UPROPERTY()
		float		m_Distance_Camera_Max = 10000.0f;
	UPROPERTY()
		float		m_Distance_Camera_Min = 100.0f;

	UPROPERTY()
		bool		m_bIsValid_Location_MouseDown_Left = false;
	UPROPERTY()
		FVector		m_Location_MouseDown_Left = FVector::ZeroVector;
	UPROPERTY()
		bool		m_bIsValid_Location_MouseDown_Right = false;
	UPROPERTY()
		FVector		m_Location_MouseDown_Right = FVector::ZeroVector;



	//Overrided from unreal
protected:
	virtual void SetupInputComponent() override;



	//Interfaces
public:
	void	MoveFocus_To(int32 _id_LocalMap);



private:
	void	OnAction_Mouse_Left_Down();
	void	OnAction_Mouse_Left_Up();
	void	OnAction_Mouse_Right_Down();
	void	OnAction_Mouse_Right_Up();

	//���콺 �� Ű �ٿ� - ��ġ���� / ��ɸ���ϰ�� ��ɽ���
	//���콺 �� Ű �� - ����� ������ �̺�Ʈ ����
	//���콺 �� Ű �ٿ� - ���ø���̰� ��ƼƼ�� ���õǾ� ������� �̵���� ���� / ������ �ൿ ���. ���콺 �¹�ư�� �������� ���ɼ��� ����. �� ��� �ش� ����� ���.
	//���콺 �� Ű �� - ���� �̵��� �巡�׷� ���ݹ�ġ ����� �� ��� �ش� ��� �߰�.

	void	Perform_Selection(void);
	void	Perform_Order(void);
	void	Perform_Plan(void);
	void	Perform_Blueprint(void);

private:
	FHitResult	Get_CursorLocation_OnLocalMap(void);
	
};
