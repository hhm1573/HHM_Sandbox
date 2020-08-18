// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TouchPanel.generated.h"



class UStaticMeshComponent;
class UBoxComponent;



UCLASS()
class HHM_SANDBOX_API ATouchPanel : public AActor	//Obsoloete
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATouchPanel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



private:
	UPROPERTY(EditAnywhere)
		UBoxComponent*				m_pComponent_Box = nullptr;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent*		m_pComponent_StaticMesh = nullptr;

};
