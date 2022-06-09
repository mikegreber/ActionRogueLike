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

bool URAttributeComponent::ApplyHealthChange(float Delta)
{
	const float OldHealth = Health;
	
	Health = FMath::Clamp(Health + Delta, 0.f, MaxHealth);
	
	OnHealthChanged.Broadcast(nullptr, this, Health, Health - OldHealth);
	
	return true;
}


