// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/RPowerupBase.h"
#include "RPowerup_Ability.generated.h"

class URAbility;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ARPowerup_Ability : public ARPowerupBase
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, Category = "Powerup")
	TSubclassOf<URAbility> AbilityClass;
	
public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
