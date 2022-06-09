// Fill out your copyright notice in the Description page of Project Settings.


#include "RHealthPotion.h"

#include "RAttributeComponent.h"

// Sets default values
ARHealthPotion::ARHealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;

	CooldownTime = 10.0f;
	HealAmount = 50.0f;
}

void ARHealthPotion::Enable()
{
	RootComponent->SetVisibility(true, true);
	SetActorEnableCollision(true);
}

void ARHealthPotion::Disable()
{
	RootComponent->SetVisibility(false, true);
	SetActorEnableCollision(false);
	
	FTimerHandle TimerHandle_Activate;
	GetWorldTimerManager().SetTimer(TimerHandle_Activate, this, &ARHealthPotion::Enable, CooldownTime);
}

void ARHealthPotion::Interact_Implementation(APawn* MyInstigator)
{
	if (URAttributeComponent* AttributeComp = Cast<URAttributeComponent>(MyInstigator->GetComponentByClass(URAttributeComponent::StaticClass())))
	{
		if (AttributeComp->ApplyHealthChange(this, HealAmount))
		{
			Disable();
		}
	}
}

