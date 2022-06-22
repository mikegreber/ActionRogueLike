// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/RAbilityEffect.h"
#include "RAbilityEffect_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URAbilityEffect_Thorns : public URAbilityEffect
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Effect")
	float ReflectionFraction;
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URAttributeComponent* OwningComp, float NewValue, float Delta);
	
public:

	URAbilityEffect_Thorns();
	
	virtual void StartAbility_Implementation(AActor* Instigator) override;
	
	virtual void StopAbility_Implementation(AActor* Instigator) override;
};
