// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RAttributeComponent.h"
#include "GameFramework/Actor.h"
#include "RTargetDummy.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ARTargetDummy : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere)
	URAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;
	
public:	
	// Sets default values for this actor's properties
	ARTargetDummy();

protected:
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URAttributeComponent* OwningComp, float NewHealth, float Delta);
};
