// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGameplayInterface.h"
#include "Actors/RPowerupBase.h"
#include "GameFramework/Actor.h"
#include "RHealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ARHealthPotion : public ARPowerupBase
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, Category = "Potion")
	float HealAmount;

	UPROPERTY(EditAnywhere, Category = "Potion")
	int32 CreditCost;
	
public:	
	// Sets default values for this actor's properties
	ARHealthPotion();

	virtual void Interact_Implementation(APawn* MyInstigator) override;
};
