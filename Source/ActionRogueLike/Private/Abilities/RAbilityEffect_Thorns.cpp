// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RAbilityEffect_Thorns.h"

#include "RAttributeComponent.h"
#include "RGameplayFunctionLibrary.h"
#include "Abilities/RAbilityComponent.h"

URAbilityEffect_Thorns::URAbilityEffect_Thorns()
{
	ReflectionFraction = 0.5;
}

void URAbilityEffect_Thorns::StartAbility_Implementation(AActor* Instigator)
{
	Super::StartAbility_Implementation(Instigator);

	if (URAttributeComponent* AttributeComp = URAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner()))
	{
		AttributeComp->OnHealthChanged.AddDynamic(this, &URAbilityEffect_Thorns::OnHealthChanged);
	}
	
}

void URAbilityEffect_Thorns::StopAbility_Implementation(AActor* Instigator)
{
	Super::StopAbility_Implementation(Instigator);

	if (URAttributeComponent* AttributeComp = URAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner()))
	{
		AttributeComp->OnHealthChanged.RemoveDynamic(this, &URAbilityEffect_Thorns::OnHealthChanged);
	}
}

void URAbilityEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, URAttributeComponent* OwningComp, float NewValue, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();
	
	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		int32 ReflectedAmount = -FMath::RoundToInt(Delta * ReflectionFraction);
		if (ReflectedAmount != 0)
		{
			// apply damage to sender
			URGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
		}
	}
}