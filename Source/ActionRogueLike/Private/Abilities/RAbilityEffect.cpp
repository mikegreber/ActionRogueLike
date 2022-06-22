// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RAbilityEffect.h"

#include "Abilities/RAbilityComponent.h"
#include "GameFramework/GameStateBase.h"


URAbilityEffect::URAbilityEffect()
{
	bAutoStart = true;
}

void URAbilityEffect::StartAbility_Implementation(AActor* Instigator)
{
	Super::StartAbility_Implementation(Instigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAbility", Instigator);
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Duration, Delegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Period, Delegate, Period, true);
	}
}

void URAbilityEffect::StopAbility_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_Period) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
	
	Super::StopAbility_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Duration);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Period);

	if (URAbilityComponent* Comp = GetOwningComponent())
	{
		Comp->RemoveAbility(this);
	}
}

float URAbilityEffect::GetTimeRemaining() const
{
	if (AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>())
	{
		return TimeStarted + Duration - GS->GetServerWorldTimeSeconds();
	}
	
	return Duration;
}

void URAbilityEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("EXECUTE"));
}
