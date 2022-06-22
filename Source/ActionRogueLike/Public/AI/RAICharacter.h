// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RAttributeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "GameFramework/Character.h"
#include "UI/RWorldUserWidget.h"
#include "RAICharacter.generated.h"

class URAbilityComponent;
class UPawnSensingComponent;

UCLASS()
class ACTIONROGUELIKE_API ARAICharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	URWorldUserWidget* ActiveHealthBar;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<URWorldUserWidget> SpottedWidgetClass;
    	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	URAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	URAbilityComponent* AbilityComp;

	UPROPERTY(EditAnywhere, Category = "AI")
	FName TargetActorKey;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;



public:
	// Sets default values for this character's properties
	ARAICharacter();

protected:
	
	virtual void PostInitializeComponents() override;

	AActor* GetTargetActor() const;
	
	void SetTargetActor(AActor* NewTarget);
	
	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastOnSeePawn();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URAttributeComponent* OwningComp, float NewValue, float Delta);
};

