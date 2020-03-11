// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/Navigation/NavLink/HHM_NavLinkProxy.h"
#include "HHM_NavLinkProxy_H2.generated.h"

/**
 * 
 */
UCLASS()
class HHM_SANDBOX_API AHHM_NavLinkProxy_H2 : public AHHM_NavLinkProxy
{
	GENERATED_BODY()

public:
	AHHM_NavLinkProxy_H2();

public:
	void ReceiveSmartLinkReached(AActor* Agent, const FVector& Destination);
};
