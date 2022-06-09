// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RCharacter.generated.h"

class URAttributeComponent;
class USpringArmComponent;
class UCameraComponent;
class URInteractionComponent;

UCLASS()
class ACTIONROGUELIKE_API ARCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	URInteractionComponent* InteractionComp;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> PrimaryProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> SecondaryProjectileClass;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<AActor> TeleportProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	URAttributeComponent* AttributeComp;
	
public:
	// Sets default values for this character's properties
	ARCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	
	void MoveForward(float X);

	void MoveRight(float X);

	void SpawnProjectile(UClass* ProjectileClass);
	
	void PrimaryAttack();

	void PrimaryAttack_TimeElapsed();

	void SecondaryAttack();

	void SecondaryAttack_TimeElapsed();

	void TeleportAbility();

	void TeleportAbility_TimeElapsed();

	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URAttributeComponent* OwningComp, float NewHealth, float Delta);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
