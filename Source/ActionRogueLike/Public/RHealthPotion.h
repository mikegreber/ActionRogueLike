// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "RHealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ARHealthPotion : public AActor, public IRGameplayInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	float HealAmount;

	UPROPERTY(EditAnywhere)
	float CooldownTime;

public:	
	// Sets default values for this actor's properties
	ARHealthPotion();

protected:

	void Enable();

	void Disable();

public:	
	virtual void Interact_Implementation(APawn* MyInstigator) override;
};
