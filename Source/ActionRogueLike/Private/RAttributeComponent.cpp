// Fill out your copyright notice in the Description page of Project Settings.


#include "RAttributeComponent.h"

// Sets default values for this component's properties
URAttributeComponent::URAttributeComponent()
{
	Health = 100;
	MaxHealth = 100;
}

bool URAttributeComponent::IsAlive() const
{
	return Health > 0;
}

bool URAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	const float OldHealth = Health;
	
	Health = FMath::Clamp(Health + Delta, 0.f, MaxHealth);
	
	if (OldHealth != Health)
	{
		OnHealthChanged.Broadcast(nullptr, this, Health, Health - OldHealth);
		return true;
	}
	
	return false;
}

URAttributeComponent* URAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<URAttributeComponent>(FromActor->GetComponentByClass(URAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool URAttributeComponent::IsActorAlive(AActor* FromActor)
{
	URAttributeComponent* AttributeComp = GetAttributes(FromActor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}
