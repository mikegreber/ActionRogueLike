// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RAbilityComponent.h"

#include "Abilities/RAbility.h"



URAbilityComponent::URAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<URAbility> AbilityClass : DefaultAbilities)
	{
		AddAbility(GetOwner(), AbilityClass);
	}
}


void URAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}

void URAbilityComponent::AddAbility(AActor* Instigator, TSubclassOf<URAbility> AbilityClass)
{
	if (!ensure(AbilityClass))
	{
		return;
	}

	URAbility* NewAbility = NewObject<URAbility>(this, AbilityClass);
	if (ensure(NewAbility))
	{
		Abilities.Add(NewAbility);
		if (NewAbility->bAutoStart && ensure(NewAbility->CanStart(Instigator)))
		{
			NewAbility->StartAbility(Instigator);
		}
	}
}

void URAbilityComponent::RemoveAbility(URAbility* AbilityToRemove)
{
	if (ensure(AbilityToRemove && !AbilityToRemove->IsRunning()))
	{
		Abilities.Remove(AbilityToRemove);
	}
	
}

bool URAbilityComponent::StartAbilityByName(AActor* Instigator, FName AbilityName)
{
	for (URAbility* Ability : Abilities)
	{
		if (Ability->AbilityName == AbilityName)
		{
			if (!Ability->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *AbilityName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}

			Ability->StartAbility(Instigator);
			return true;
		}
	}

	return false;
}

bool URAbilityComponent::StopAbilityByName(AActor* Instigator, FName AbilityName)
{
	for (URAbility* Ability : Abilities)
	{
		if (Ability && Ability->AbilityName == AbilityName)
		{
			if (Ability->IsRunning())
			{
				Ability->StopAbility(Instigator);
				return true;
			}
		}
	}

	return false;
}

URAbilityComponent* URAbilityComponent::GetAbilityComponent(AActor* FromActor)
{
	return Cast<URAbilityComponent>(FromActor->GetComponentByClass(StaticClass()));
}