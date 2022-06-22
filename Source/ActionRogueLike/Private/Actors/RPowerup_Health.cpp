// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RPowerup_Health.h"

#include "RAttributeComponent.h"
#include "RPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

ARPowerup_Health::ARPowerup_Health()
{
	HealAmount = 50.f;
	CreditCost = 10;
}

void ARPowerup_Health::Interact_Implementation(APawn* InstigatorPawn)
{
	if (URAttributeComponent* AttributeComp = URAttributeComponent::GetAttributes(InstigatorPawn))
	{
		if (ARPlayerState* PS = InstigatorPawn->GetPlayerState<ARPlayerState>())
		{
			if (PS->GetCredits() >= CreditCost)
			{
				if (AttributeComp->ApplyHealthChange(this, HealAmount))
				{
					PS->RemoveCredits(CreditCost);
					HideAndCooldownPowerup();
				}
			}
		}
	}
}

FText ARPowerup_Health::GetInteractionText_Implementation(APawn* InstigatorPawn)
{
	URAttributeComponent* AttributeComp = URAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComp && AttributeComp->IsFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
	}
	
	return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximum."), CreditCost);
}


#undef LOCTEXT_NAMESPACE