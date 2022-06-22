// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "RAbility.generated.h"

class URAttributeComponent;
class URAbilityComponent;

USTRUCT()
struct FAbilityRepData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API URAbility : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;
	
	// tags added to OwningActor when ability starts, removed when ability ends
	UPROPERTY(EditDefaultsOnly, Category = "Tags");
	FGameplayTagContainer GrantsTags;

	// can only start action if OwningActor has none of these tags
	UPROPERTY(EditDefaultsOnly, Category = "Tags");
	FGameplayTagContainer BlockedTags;

	UPROPERTY(Replicated)
	URAbilityComponent* AbilityComp;

	UPROPERTY(ReplicatedUsing=OnRep_RepData);
	FAbilityRepData RepData;

	UPROPERTY(Replicated)
	float TimeStarted;

	UFUNCTION()
	void OnRep_RepData();
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FName AbilityName;
	
	/* Start immediately when added to an ability component */
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	bool bAutoStart;
	
	void Initialize(URAbilityComponent* NewAbilityComp);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	URAbilityComponent* GetOwningComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	URAttributeComponent* GetOwningAttributes() const;
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool IsRunning() const;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	void StartAbility(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ability")
	void StopAbility(AActor* Instigator);

	virtual UWorld* GetWorld() const override;

	virtual bool IsSupportedForNetworking() const override { return true; } 
};
