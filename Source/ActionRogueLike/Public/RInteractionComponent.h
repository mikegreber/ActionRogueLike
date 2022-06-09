// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API URInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	void PrimaryInteract();
	
	// Sets default values for this component's properties
	URInteractionComponent();
};
