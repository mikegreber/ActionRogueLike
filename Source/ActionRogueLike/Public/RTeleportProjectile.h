// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RProjectile.h"
#include "RTeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ARTeleportProjectile : public ARProjectile
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	UParticleSystem* TeleportCloseEffect;
	
	UPROPERTY(EditAnywhere)
	UParticleSystem* TeleportEnterEffect;

	UPROPERTY(EditAnywhere)
	float TeleportTime;

	UPROPERTY(EditAnywhere)
	float TeleportDelay;

public:
	
	ARTeleportProjectile();

protected:
	
	void Teleport();

	void Teleport_TimeElapsed();

	virtual void OnHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, FVector Vector, const FHitResult& HitResult) override;
	
	virtual void BeginPlay() override;


private:
	
	FTimerHandle TimerHandle_Teleport;
};
