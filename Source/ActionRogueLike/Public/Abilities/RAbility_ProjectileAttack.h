// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/RAbility.h"
#include "RAbility_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URAbility_ProjectileAttack : public URAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(VisibleAnywhere, Category =" Attack")
	FName HandSocketName;

public:

	URAbility_ProjectileAttack();
	
	virtual void StartAbility_Implementation(AActor* Instigator) override;
	
	virtual void StopAbility_Implementation(AActor* Instigator) override;

protected:

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);
	
};
