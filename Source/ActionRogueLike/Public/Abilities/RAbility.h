// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "RAbility.generated.h"

class URAbilityComponent;

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API URAbility : public UObject
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category = "Ability")
	URAbilityComponent* GetOwningComponent() const;

	// tags added to OwningActor when ability starts, removed when ability ends
	UPROPERTY(EditDefaultsOnly, Category = "Tags");
	FGameplayTagContainer GrantsTags;

	// can only start action if OwningActor has none of these tags
	UPROPERTY(EditDefaultsOnly, Category = "Tags");
	FGameplayTagContainer BlockedTags;

	bool bIsRunning;
	
public:

	/* Start immediately when added to an ability component */
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool IsRunning() const;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	void StartAbility(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ability")
	void StopAbility(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FName AbilityName;

	virtual UWorld* GetWorld() const override;
};
