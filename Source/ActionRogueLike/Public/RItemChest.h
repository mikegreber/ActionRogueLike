// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "RItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ARItemChest : public AActor, public IRGameplayInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float TargetPitch;
	
	virtual void Interact_Implementation(APawn* Instigator) override;
	
protected:
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;
	
public:	
	// Sets default values for this actor's properties
	ARItemChest();

};
