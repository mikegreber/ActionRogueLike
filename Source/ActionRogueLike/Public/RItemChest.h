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

	virtual void OnActorLoaded_Implementation() override;
protected:
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	UPROPERTY(ReplicatedUsing = OnRep_LidOpen, BlueprintReadOnly, SaveGame)
	bool bLidOpen;

	UFUNCTION()
	void OnRep_LidOpen();
	
public:	
	// Sets default values for this actor's properties
	ARItemChest();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
