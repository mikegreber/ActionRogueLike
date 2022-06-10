// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RPowerupBase.h"

// Sets default values
ARPowerupBase::ARPowerupBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	
	CooldownTime = 10.0f;
}

void ARPowerupBase::Enable()
{
	RootComponent->SetVisibility(true, true);
	SetActorEnableCollision(true);
}

void ARPowerupBase::Disable()
{
	RootComponent->SetVisibility(false, true);
	SetActorEnableCollision(false);
	
	FTimerHandle TimerHandle_Activate;
	GetWorldTimerManager().SetTimer(TimerHandle_Activate, this, &ARPowerupBase::Enable, CooldownTime);
}

