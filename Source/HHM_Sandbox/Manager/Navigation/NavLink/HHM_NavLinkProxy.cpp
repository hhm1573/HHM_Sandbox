// Fill out your copyright notice in the Description page of Project Settings.

#include "HHM_NavLinkProxy.h"

#include "AI/Navigation/NavLinkDefinition.h"
#include "NavigationSystem/Public/NavLinkCustomComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/Controller.h"

#include "Entity/HHM_Entity.h"



AHHM_NavLinkProxy::AHHM_NavLinkProxy(void) {
	bSmartLinkIsRelevant = true;
	SetSmartLinkEnabled(true);
	//SmartLinkComp->SetMoveReachedLink(this, &ANavLinkProxy::NotifySmartLinkReached);
	UNavLinkCustomComponent* pSmartLinkComp = GetSmartLinkComp();
	if (pSmartLinkComp != nullptr) {
		pSmartLinkComp->SetMoveReachedLink(this, &AHHM_NavLinkProxy::NotifySmartLinkReached);
	}
}



void AHHM_NavLinkProxy::NotifySmartLinkReached(UNavLinkCustomComponent* LinkComp, UObject* PathingAgent, const FVector& DestPoint)
{
	UPathFollowingComponent* PathComp = Cast<UPathFollowingComponent>(PathingAgent);
	if (PathComp)
	{
		AActor* PathOwner = PathComp->GetOwner();
		AController* ControllerOwner = Cast<AController>(PathOwner);
		if (ControllerOwner)
		{
			PathOwner = Cast<AActor>(ControllerOwner->GetPawn());
		}

		ReceiveSmartLinkReached(PathOwner, DestPoint);
		OnSmartLinkReached.Broadcast(PathOwner, DestPoint);
	}
}

void AHHM_NavLinkProxy::ReceiveSmartLinkReached(AActor* Agent, const FVector& Destination) {
	//Get distance from input Destination to Left/Right link point location
	FVector	LinkWorldLocation = GetActorLocation();
	FVector LinkPoint_Left = PointLinks[0].Left + LinkWorldLocation;
	FVector LinkPoint_Right = PointLinks[0].Right + LinkWorldLocation;
	FVector	DestDistance_Left = Destination - LinkPoint_Left;
	FVector	DestDistance_Right = Destination - LinkPoint_Right;
	float	Length_DistanceLeft = FMath::Abs<float>(DestDistance_Left.Size());
	float	Length_DistanceRight = FMath::Abs<float>(DestDistance_Right.Size());
	
	//Get exactly correct Start/End location
	//Because sometimes agent can trigger link not exactly same point as it starts
	FVector	JumpStartPoint = (Length_DistanceLeft > Length_DistanceRight) ? LinkPoint_Left : LinkPoint_Right;
	FVector JumpEndPoint = Destination;

	AHHM_Entity*	pEntity = Cast<AHHM_Entity>(Agent);
	if (pEntity == nullptr) {
		//Exception
		return;
	}

	//TargetPoint is higher than StartPoint! that mean agent should do jump
	if (JumpEndPoint.Z > JumpStartPoint.Z) {	
		//Do Jump!!!
		pEntity->HHM_Jump(JumpStartPoint, JumpEndPoint, m_Height);
	}
	else {
		//Fell off!!!
		pEntity->HHM_FallOff(JumpStartPoint, JumpEndPoint, m_Height);
	}

	
}



void AHHM_NavLinkProxy::Set_LinkData(const FVector& left, const FVector& right) {
	PointLinks[0].Left = left;
	PointLinks[0].Right = right;
	UNavLinkCustomComponent* pSmartLinkComp = GetSmartLinkComp();
	pSmartLinkComp->SetLinkData(left, right, ENavLinkDirection::BothWays);
}