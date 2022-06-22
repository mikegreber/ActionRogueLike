// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RAbility.h"

#include "RAttributeComponent.h"
#include "Abilities/RAbilityComponent.h"
#include "ActionRogueLike/ActionRogueLike.h"
#include "Net/UnrealNetwork.h"

void URAbility::Initialize(URAbilityComponent* NewAbilityComp)
{
	AbilityComp = NewAbilityComp;
}

bool URAbility::CanStart_Implementation(AActor* Instigator)
{
	if (RepData.bIsRunning)
	{
		return false;
	}
	
	return !GetOwningComponent()->ActiveGameplayTags.HasAny(BlockedTags);
}

void URAbility::StartAbility_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Start Ability: %s"), *GetNameSafe(this));
	// LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *AbilityName.ToString()), FColor::Green);

	URAbilityComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	// server only
	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}
	
	GetOwningComponent()->OnAbilityStarted.Broadcast(GetOwningComponent(), this);
}

void URAbility::StopAbility_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stop Ability: %s"), *GetNameSafe(this));
	// LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *AbilityName.ToString()), FColor::White);
	
	URAbilityComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnAbilityStopped.Broadcast(GetOwningComponent(), this);
}

URAbilityComponent* URAbility::GetOwningComponent() const
{
	return AbilityComp;
}

URAttributeComponent* URAbility::GetOwningAttributes() const
{
	return Cast<URAttributeComponent>(GetOwningComponent()->GetOwner()->GetComponentByClass(URAttributeComponent::StaticClass()));
}

UWorld* URAbility::GetWorld() const
{
	// outer is set when creating ability with NewObject<T>()
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

void URAbility::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAbility(RepData.Instigator);
	}
	else
	{
		StopAbility(RepData.Instigator);
	}
}

bool URAbility::IsRunning() const
{
	return RepData.bIsRunning;
}

void URAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URAbility, RepData);
	DOREPLIFETIME(URAbility, AbilityComp);
	DOREPLIFETIME(URAbility, TimeStarted);
}
