// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_ItemActor.h"

#include "Components/StaticMeshComponent.h"

#include "UObject/ConstructorHelpers.h"



// Sets default values
AHHM_ItemActor::AHHM_ItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	UStaticMeshComponent* pComponent_StaticMesh = nullptr;
	pComponent_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	if (pComponent_StaticMesh == nullptr) {
		//Exception
	}
	else {
		static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh_Asset(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
		if (StaticMesh_Asset.Succeeded() == true) {
			pComponent_StaticMesh->SetStaticMesh(StaticMesh_Asset.Object);
			m_pComponent_StaticMesh = pComponent_StaticMesh;
			m_pComponent_StaticMesh->SetupAttachment(RootComponent);
		}
		else {
			//Exception No Mesh Asset Found
		}
	}
}

// Called when the game starts or when spawned
void AHHM_ItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHHM_ItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

