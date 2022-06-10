// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/RPowerupBase.h"
#include "RPowerup_Credits.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ARPowerup_Credits : public ARPowerupBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Powerup")
	int32 CreditAmount;

public:
	ARPowerup_Credits();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
