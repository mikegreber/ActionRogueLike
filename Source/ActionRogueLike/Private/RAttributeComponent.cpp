// Fill out your copyright notice in the Description page of Project Settings.


#include "RAttributeComponent.h"

#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable CVarDamageMultiplier(TEXT("ARL..DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component"), ECVF_Cheat);




// Sets default values for this component's properties
URAttributeComponent::URAttributeComponent()
{
	Health = 100;
	MaxHealth = 100;

	Rage = 0;
	MaxRage = 100;

	SetIsReplicatedByDefault(true);
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
	float NewHealth = FMath::Clamp(Health + Delta, 0.f, MaxHealth);
	float ActualDelta = NewHealth - OldHealth;
	
	// server only
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if (ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}

		// killed
		if (ActualDelta < 0.0f && Health == 0)
		{
			ARGameModeBase* GM = GetWorld()->GetAuthGameMode<ARGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}
	
	return ActualDelta != 0.0f;
}

bool URAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	float OldHealth = Rage;
	
	Rage = FMath::Clamp(Rage + Delta, 0.f, MaxRage);
	
	float ActualDelta = Rage - OldHealth;
	
	if (ActualDelta != 0.0f)
	{
		MulticastRageChanged(InstigatorActor, Rage, ActualDelta);
	}
	
	return ActualDelta != 0.0f;
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

void URAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
	
	if (Delta < 0)
	{
		Rage -= Delta;
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, -Delta);
	}
}

void URAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}

void URAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URAttributeComponent, Health);
	DOREPLIFETIME(URAttributeComponent, MaxHealth);
	DOREPLIFETIME(URAttributeComponent, Rage);
	DOREPLIFETIME(URAttributeComponent, MaxRage);
}