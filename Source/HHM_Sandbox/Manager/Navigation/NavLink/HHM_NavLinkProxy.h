// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "HHM_NavLinkProxy.generated.h"

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API AHHM_NavLinkProxy : public ANavLinkProxy
{
	GENERATED_BODY()

public:
	AHHM_NavLinkProxy();

protected:
	int32	m_Height = 0;

protected:
	void NotifySmartLinkReached(UNavLinkCustomComponent* LinkComp, UObject* PathingAgent, const FVector& DestPoint);
public:
	void ReceiveSmartLinkReached(AActor* Agent, const FVector& Destination);

public:
	void Set_LinkData(const FVector& left, const FVector& right);
	void Set_Height(int32 heightGap) { m_Height = heightGap; }
	
};
