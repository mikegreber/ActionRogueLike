// Fill out your copyright notice in the Description page of Project Settings.


#include "RHealthPotion.h"

#include "RAttributeComponent.h"
#include "RPlayerState.h"

// Sets default values
ARHealthPotion::ARHealthPotion()
{
	CreditCost = 10;
	HealAmount = 50.0f;
}


void ARHealthPotion::Interact_Implementation(APawn* MyInstigator)
{
	if (URAttributeComponent* AttributeComp = Cast<URAttributeComponent>(MyInstigator->GetComponentByClass(URAttributeComponent::StaticClass())))
	{
		if (ARPlayerState* PS = MyInstigator->GetPlayerState<ARPlayerState>())
		{
			if (PS->GetCredits() >= CreditCost && AttributeComp->ApplyHealthChange(this, HealAmount))
			{
				PS->RemoveCredits(CreditCost);
				Disable();
			}
		}
	}
}

