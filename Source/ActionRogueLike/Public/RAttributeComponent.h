// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGameModeBase.h"
#include "AI/RBTService_CheckLowHealth.h"
#include "Components/ActorComponent.h"
#include "RAttributeComponent.generated.h"

class URAttributeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, URAttributeComponent*, OwningComp, float, NewValue, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API URAttributeComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category="Attributes")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category="Attributes")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category="Attributes")
	float Rage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category="Attributes")
	float MaxRage;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRageChanged(AActor* InstigatorActor, float NewHealth, float Delta);
	
public:	
	
	URAttributeComponent();
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static URAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool IsAlive() const;
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyRageChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool IsFullHealth() const { return Health == MaxHealth; };

	
};
