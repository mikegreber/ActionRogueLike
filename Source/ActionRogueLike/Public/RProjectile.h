// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ARProjectile : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UProjectileMovementComponent* MovementComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* EffectComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioComponent* AudioComp;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* ImpactVFX;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UCameraShakeBase> ImpactShake;

	UPROPERTY(EditAnywhere, Category = "Effects")
	float ImpactShakeInnerRadius;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	float ImpactShakeOuterRadius;

public:
	
	// Sets default values for this actor's properties
	ARProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintNativeEvent, Category = "Projectile")
	void Explode();
	
};
