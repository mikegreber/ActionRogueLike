// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/RPowerupBase.h"
#include "RPowerup_Health.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ARPowerup_Health : public ARPowerupBase
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditAnywhere, Category = "Powerup")
	int32 CreditCost;
	
	UPROPERTY(EditAnywhere, Category = "Powerup")
	float HealAmount;

public:
	ARPowerup_Health();
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractionText_Implementation(APawn* InstigatorPawn) override;
};
