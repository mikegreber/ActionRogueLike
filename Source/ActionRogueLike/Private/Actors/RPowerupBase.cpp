// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RPowerupBase.h"

#include "Net/UnrealNetwork.h"

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

	bIsActive = true;

	bReplicates = true;
}

void ARPowerupBase::OnActorLoaded_Implementation()
{}

void ARPowerupBase::Interact_Implementation(APawn* InstigatorPawn)
{}

FText ARPowerupBase::GetInteractionText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void ARPowerupBase::ActivatePowerup()
{
	SetPowerupState(true);
}

void ARPowerupBase::HideAndCooldownPowerup()
{
	SetPowerupState(false);
	
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ARPowerupBase::ActivatePowerup, CooldownTime);
}

void ARPowerupBase::SetPowerupState(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	OnRep_Enabled();
}



void ARPowerupBase::OnRep_Enabled()
{
	// set collision and visibility
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive, true);
}

void ARPowerupBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPowerupBase, bIsActive);
}