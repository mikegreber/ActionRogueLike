// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGameplayInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "RPowerupBase.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ARPowerupBase : public AActor, public IRGameplayInterface
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	float CooldownTime;
	
public:	
	// Sets default values for this actor's properties
	ARPowerupBase();

protected:

	void Enable();

	void Disable();
};
