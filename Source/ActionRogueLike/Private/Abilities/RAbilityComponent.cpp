// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RAbilityComponent.h"

#include "Abilities/RAbility.h"
#include "ActionRogueLike/ActionRogueLike.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"


DECLARE_CYCLE_STAT(TEXT("StartAbilityByName"), STAT_StartAbilityByName, STATGROUP_ROGUELIKE);

URAbilityComponent::URAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void URAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	// server only
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<URAbility> AbilityClass : DefaultAbilities)
		{
			AddAbility(GetOwner(), AbilityClass);
		}
	}
}

void URAbilityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<URAbility*> AbilitiesCopy = Abilities;
	for (URAbility* Ability : AbilitiesCopy)
	{
		if (Ability && Ability->IsRunning())
		{
			Ability->StopAbility(GetOwner());
		}
	}
	
	Super::EndPlay(EndPlayReason);
}

void URAbilityComponent::ServerStopAbility_Implementation(AActor* Instigator, FName AbilityName)
{
	StopAbilityByName(Instigator, AbilityName);
}

void URAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	for (URAbility* Ability : Abilities)
	{
		FColor TextColor = Ability->IsRunning() ? FColor::Blue : FColor::White;
		FString AbilityMsg = FString::Printf(TEXT("[%s] Ability: %s"), *GetNameSafe(GetOuter()), *GetNameSafe(Ability));
		
		LogOnScreen(this, AbilityMsg, TextColor, 0.0f);
	}
}

URAbility* URAbilityComponent::GetAbility(TSubclassOf<URAbility> AbilityClass)
{
	for (URAbility* Ability: Abilities)
	{
		if (Ability && Ability->IsA(AbilityClass))
		{
			return Ability;
		}
	}
		
	return nullptr;
}

void URAbilityComponent::AddAbility(AActor* Instigator, TSubclassOf<URAbility> AbilityClass)
{
	if (!ensure(AbilityClass))
	{
		return;
	}

	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction. [Class: %s]"), *GetNameSafe(AbilityClass));
	}

	URAbility* NewAbility = NewObject<URAbility>(GetOwner(), AbilityClass);
	if (ensure(NewAbility))
	{
		NewAbility->Initialize(this);
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
	SCOPE_CYCLE_COUNTER(STAT_StartAbilityByName);
	
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

			// call client only
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAbility(Instigator, AbilityName);
			}

			// Bookmark for Unreal Insights
			TRACE_BOOKMARK(TEXT("StartAbility::%s"), *GetNameSafe(Ability));

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
				// call client only
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAbility(Instigator, AbilityName);
				}
				
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

void URAbilityComponent::ServerStartAbility_Implementation(AActor* Instigator, FName AbilityName)
{
	StartAbilityByName(Instigator, AbilityName);
}


bool URAbilityComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (URAbility* Ability : Abilities)
	{
		if (Ability)
		{
			WroteSomething |= Channel->ReplicateSubobject(Ability, *Bunch, *RepFlags);
		}
	}
	return WroteSomething;
}

void URAbilityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URAbilityComponent, Abilities);
}
