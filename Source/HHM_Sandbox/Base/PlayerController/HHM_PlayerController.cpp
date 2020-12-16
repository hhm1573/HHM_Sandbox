// Fill out your copyright notice in the Description page of Project Settings.

#include "HHM_PlayerController.h"

#include "Engine/Public/CollisionQueryParams.h"



AHHM_PlayerController::AHHM_PlayerController(void) {
	bShowMouseCursor = true;
	bEnableClickEvents = true;
}



void AHHM_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("MouseClick_Left"), EInputEvent::IE_Pressed, this, &AHHM_PlayerController::OnAction_Mouse_Left_Down);
	InputComponent->BindAction(TEXT("MouseClick_Left"), EInputEvent::IE_Released, this, &AHHM_PlayerController::OnAction_Mouse_Left_Up);
	InputComponent->BindAction(TEXT("MouseClick_Right"), EInputEvent::IE_Pressed, this, &AHHM_PlayerController::OnAction_Mouse_Right_Down);
	InputComponent->BindAction(TEXT("MouseClick_Right"), EInputEvent::IE_Released, this, &AHHM_PlayerController::OnAction_Mouse_Right_Up);
}



void AHHM_PlayerController::MoveFocus_To(int32 _id_LocalMap)
{
	//To do : Cancel Controller Mode upon changing focus
}



void AHHM_PlayerController::OnAction_Mouse_Left_Down()
{
	if (m_eControllerMode == EHHM_ControllerMode::ControllerMode_Selection || m_eControllerMode == EHHM_ControllerMode::ControllerMode_Plan
		|| m_eControllerMode == EHHM_ControllerMode::ControllerMode_Blueprint) {

		FHitResult HitResult = Get_CursorLocation_OnLocalMap();
		if (HitResult.bBlockingHit == true) {
			m_bIsValid_Location_MouseDown_Left = true;
			m_Location_MouseDown_Left = HitResult.ImpactPoint;
		}
		else {
			m_bIsValid_Location_MouseDown_Left = false;
			m_Location_MouseDown_Left = FVector::ZeroVector;
		}

	}
	else if (m_eControllerMode == EHHM_ControllerMode::ControllerMode_Order) {
		// To do : Ordering selected entities. if no entity selected, Do nothing. than Cancel the mode
		Perform_Order();
	}
	else {
		//Exception Unknown Controller Mode
		return;
	}
}

void AHHM_PlayerController::OnAction_Mouse_Left_Up()
{
	switch (m_eControllerMode)
	{
	case EHHM_ControllerMode::ControllerMode_Selection:
		Perform_Selection();
		break;
	case EHHM_ControllerMode::ControllerMode_Order:
		break;
	case EHHM_ControllerMode::ControllerMode_Plan:
		Perform_Plan();
		break;
	case EHHM_ControllerMode::ControllerMode_Blueprint:
		Perform_Blueprint();
		break;
	case EHHM_ControllerMode::ControllerMode_End:
		break;
	default:
		break;
	}
}

void AHHM_PlayerController::OnAction_Mouse_Right_Down()
{
	if (m_eControllerMode == EHHM_ControllerMode::ControllerMode_Selection) {
		//if order-able entity selected, move to target location
	}
	else {

	}
}

void AHHM_PlayerController::OnAction_Mouse_Right_Up()
{
}



void AHHM_PlayerController::Perform_Selection(void)
{
	//HHM Note : don't forget shift mode
}

void AHHM_PlayerController::Perform_Order(void)
{
}

void AHHM_PlayerController::Perform_Plan(void)
{
}

void AHHM_PlayerController::Perform_Blueprint(void)
{
}



FHitResult AHHM_PlayerController::Get_CursorLocation_OnLocalMap(void)
{
	FHitResult	HitResult_Return = FHitResult();



	//Deproject mouse location
	FVector Location_MousePosition = FVector(0.0f);
	FVector Direction_MousePosition = FVector(0.0f);

	bool IsSucceed_Deprojection = DeprojectMousePositionToWorld(Location_MousePosition, Direction_MousePosition);
	if (IsSucceed_Deprojection == false) {
		//Exception Critical
		return HitResult_Return;
	}

	//Prepare End Location for line-tracing
	FVector Vec_ToEnd = Direction_MousePosition * (m_Distance_Camera_Max * 3.0f);
	FVector Location_End = Location_MousePosition + Vec_ToEnd;

	

	//Perform Line-Trace
	FCollisionQueryParams CollisionParams = FCollisionQueryParams();
	bool IsHit = ActorLineTraceSingle(HitResult_Return, Location_MousePosition, Location_End, ECollisionChannel::ECC_EngineTraceChannel1, CollisionParams);

	return HitResult_Return;
}
