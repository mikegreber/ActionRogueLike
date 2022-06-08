// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RExplosiveBarrel.generated.h"

class URadialForceComponent;
UCLASS()
class ACTIONROGUELIKE_API ARExplosiveBarrel : public AActor
{
	GENERATED_BODY()

	
public:
	
	// Sets default values for this actor's properties
	ARExplosiveBarrel();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* ForceComp;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, FVector Vector, const FHitResult& HitResult);
	
	// UFUNCTION()
	// void OnHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, UE::Math::TVector<double> Vector, const FHitResult& HitResult);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
