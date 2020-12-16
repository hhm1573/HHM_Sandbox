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

	//마우스 좌 키 다운 - 위치저장 / 명령모드일경우 명령실행
	//마우스 좌 키 업 - 명령을 제외한 이벤트 실행
	//마우스 우 키 다운 - 선택모드이고 엔티티가 선택되어 있을경우 이동명령 실행 / 나머지 행동 취소. 마우스 좌버튼이 눌려있을 가능성이 있음. 이 경우 해당 명령을 취소.
	//마우스 우 키 업 - 만약 이동시 드래그로 간격배치 기능을 할 경우 해당 기능 추가.

	void	Perform_Selection(void);
	void	Perform_Order(void);
	void	Perform_Plan(void);
	void	Perform_Blueprint(void);

private:
	FHitResult	Get_CursorLocation_OnLocalMap(void);
	
};
