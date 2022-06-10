// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RProjectile.h"
#include "Abilities/RAbility.h"
#include "RMagicProjectile.generated.h"

class URAbilityEffect;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class ACTIONROGUELIKE_API ARMagicProjectile : public ARProjectile
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Damage")
	float DamageAmount;
	
	UPROPERTY(EditAnywhere, Category = "Damage")
	FGameplayTag ParryTag;

	UPROPERTY(EditAnywhere, Category = "Damage")
	TSubclassOf<URAbilityEffect> BurningEffectClass;

public:
	
	ARMagicProjectile();

protected:

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
};
