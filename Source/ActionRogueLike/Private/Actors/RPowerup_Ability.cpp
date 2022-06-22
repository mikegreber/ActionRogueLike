// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RPowerup_Ability.h"

#include "Abilities/RAbility.h"
#include "Abilities/RAbilityComponent.h"

void ARPowerup_Ability::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	URAbilityComponent* AbilityComp = URAbilityComponent::GetAbilityComponent(InstigatorPawn);
	if (AbilityComp && ensure(AbilityClass))
	{
		if (!AbilityComp->GetAbility(AbilityClass))
		{
			AbilityComp->AddAbility(InstigatorPawn, AbilityClass);
			HideAndCooldownPowerup();
		}
	}
}
