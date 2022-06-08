// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RProjectile.h"
#include "RBlackholeProjectile.generated.h"

class URadialForceComponent;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API ARBlackholeProjectile : public ARProjectile
{
	GENERATED_BODY()

protected:
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	// USphereComponent* SphereComp;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	// UProjectileMovementComponent* MovementComp;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	// UParticleSystemComponent* EffectComp;

	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* ForceComp;
	
public:
	
	// Sets default values for this actor's properties
	ARBlackholeProjectile();

protected:
	
	virtual void OnBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
