// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RAbility.h"

#include "Abilities/RAbilityComponent.h"

bool URAbility::CanStart_Implementation(AActor* Instigator)
{
	if (bIsRunning)
	{
		return false;
	}
	
	return !GetOwningComponent()->ActiveGameplayTags.HasAny(BlockedTags);
}

void URAbility::StartAbility_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Start Ability: %s"), *GetNameSafe(this));

	URAbilityComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void URAbility::StopAbility_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stop Ability: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);
	
	URAbilityComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

URAbilityComponent* URAbility::GetOwningComponent() const
{
	return Cast<URAbilityComponent>(GetOuter());
}

UWorld* URAbility::GetWorld() const
{
	// outer is set when creating ability with NewObject<T>()
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}

bool URAbility::IsRunning() const
{
	return bIsRunning;
}