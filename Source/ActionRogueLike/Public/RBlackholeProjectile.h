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
	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* ForceComp;
	
public:
	
	// Sets default values for this actor's properties
	ARBlackholeProjectile();

protected:

	UFUNCTION()
	void OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
