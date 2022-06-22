// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RAnimInstance.h"

#include "GameplayTagContainer.h"
#include "Abilities/RAbilityComponent.h"

void URAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* OwningActor = GetOwningActor())
	{
		AbilityComp = URAbilityComponent::GetAbilityComponent(OwningActor);
	}
}

void URAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");

	if (AbilityComp)
	{
		bIsStunned = AbilityComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
