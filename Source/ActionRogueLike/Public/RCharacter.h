// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RCharacter.generated.h"

class URAbilityComponent;
class URAttributeComponent;
class USpringArmComponent;
class UCameraComponent;
class URInteractionComponent;

UCLASS()
class ACTIONROGUELIKE_API ARCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	URInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URAbilityComponent* AbilityComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	URAttributeComponent* AttributeComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Effects");
	FName TimeToHitParamName;

public:
	// Sets default values for this character's properties
	ARCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;
	
	void MoveForward(float X);

	void MoveRight(float X);
	
	void SprintStart();

	void SprintStop();
	
	void PrimaryAttack();
	
	void SecondaryAttack();
	
	void DashAbility();
	
	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URAttributeComponent* OwningComp, float NewHealth, float Delta);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.f);
};

 
