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

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Powerup")
	float CooldownTime;

	UPROPERTY(ReplicatedUsing=OnRep_Enabled)
	bool bIsActive;

	UFUNCTION()
	void OnRep_Enabled();
	
	FTimerHandle TimerHandle_RespawnTimer;

public:	
	// Sets default values for this actor's properties
	ARPowerupBase();

	virtual void OnActorLoaded_Implementation() override;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractionText_Implementation(APawn* InstigatorPawn) override;
	
protected:

	void ActivatePowerup();

	void HideAndCooldownPowerup();

	void SetPowerupState(bool bNewIsActive);

};
