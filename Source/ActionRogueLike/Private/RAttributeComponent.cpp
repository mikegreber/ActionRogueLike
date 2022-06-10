// Fill out your copyright notice in the Description page of Project Settings.


#include "RAttributeComponent.h"

static TAutoConsoleVariable CVarDamageMultiplier(TEXT("ARL..DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component"), ECVF_Cheat);

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
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}

	float OldHealth = Health;
	
	Health = FMath::Clamp(Health + Delta, 0.f, MaxHealth);
	
	float ActualDelta = Health - OldHealth;

	
	
	// killed
	if (ActualDelta < 0.0f && Health == 0)
	{
		ARGameModeBase* GM = GetWorld()->GetAuthGameMode<ARGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
		
	if (ActualDelta != 0.0f)
	{
		OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);
		return true;
	}
	
	return false;
}

bool URAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -Health);
}

URAttributeComponent* URAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<URAttributeComponent>(FromActor->GetComponentByClass(StaticClass()));
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
