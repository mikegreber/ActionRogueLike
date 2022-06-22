// Fill out your copyright notice in the Description page of Project Settings.


#include "RItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ARItemChest::ARItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;
	
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(RootComponent);

	TargetPitch = 110;

	bReplicates = true;
}


void ARItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpen = !bLidOpen;
	OnRep_LidOpen();
}

void ARItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpen();
}

void ARItemChest::OnRep_LidOpen()
{
	LidMesh->SetRelativeRotation(FRotator(bLidOpen ? TargetPitch : 0.0f,0.0f,0.0f));
}

void ARItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARItemChest, bLidOpen);
}