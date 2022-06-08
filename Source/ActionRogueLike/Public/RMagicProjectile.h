// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RProjectile.h"
#include "RMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class ACTIONROGUELIKE_API ARMagicProjectile : public ARProjectile
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere)
	UParticleSystem* HitEffect;
	
public:
	
	// Sets default values for this actor's properties
	ARMagicProjectile();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
