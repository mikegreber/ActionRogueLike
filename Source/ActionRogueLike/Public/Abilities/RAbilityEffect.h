// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/RAbility.h"
#include "RAbilityEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URAbilityEffect : public URAbility
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	/* Time between 'ticks' to apply effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;	
	
public:
	URAbilityEffect();
	
	virtual void StartAbility_Implementation(AActor* Instigator) override;
	
	virtual void StopAbility_Implementation(AActor* Instigator) override;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* InstigatorActor);
	
	FTimerHandle TimerHandle_Period;
	FTimerHandle TimerHandle_Duration;

};
