// Fill out your copyright notice in the Description page of Project Settings.

#include "HHM_Manager.h"

// Sets default values
AHHM_Manager::AHHM_Manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AHHM_Manager::BeginPlay()
{
	Super::BeginPlay();
	
	m_pGameInstance = GetGameInstance();
}

// Called every frame
void AHHM_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

