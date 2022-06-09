// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RAttributeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "GameFramework/Character.h"
#include "RAICharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class ACTIONROGUELIKE_API ARAICharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	URAttributeComponent* AttributeComp;

	UPROPERTY(EditAnywhere, Category = "AI")
	FName TargetActorKey;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

public:
	// Sets default values for this character's properties
	ARAICharacter();

protected:
	
	virtual void PostInitializeComponents() override;

	void SetTargetActor(AActor* NewTarget);
	
	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URAttributeComponent* OwningComp, float NewValue, float Delta);
};

