// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RAbilityEffect.h"
#include "Components/ActorComponent.h"
#include "RAbilityComponent.generated.h"

class URAbility;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API URAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
	static URAbilityComponent* GetAbilityComponent(AActor* FromActor);

protected:

	// granted abilities at game start
	UPROPERTY(EditAnywhere, Category = "Ability")
	TArray<TSubclassOf<URAbility>> DefaultAbilities;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;
	
public:	
	// Sets default values for this component's properties
	URAbilityComponent();

	
protected:

	UPROPERTY()
	TArray<URAbility*> Abilities;
	
	virtual void BeginPlay() override;

public:
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void AddAbility(AActor* Instigator, TSubclassOf<URAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void RemoveAbility(URAbility* AbilityToRemove);
	
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool StartAbilityByName(AActor* Instigator, FName AbilityName);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool StopAbilityByName(AActor* Instigator, FName AbilityName);

	
};
