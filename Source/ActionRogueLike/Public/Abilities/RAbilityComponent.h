// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RAbilityEffect.h"
#include "Components/ActorComponent.h"
#include "RAbilityComponent.generated.h"

class URAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityStateChanged, URAbilityComponent*, OwningComp, URAbility*, Ability);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API URAbilityComponent : public UActorComponent
{
	GENERATED_BODY()


	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

protected:

	// granted abilities at game start
	UPROPERTY(EditAnywhere, Category = "Ability")
	TArray<TSubclassOf<URAbility>> DefaultAbilities;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Actions")
	TArray<URAbility*> Abilities;
	
public:
	
	URAbilityComponent();
	
protected:
	
	virtual void BeginPlay() override;

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	UFUNCTION(Server, Reliable)
	void ServerStartAbility(AActor* Instigator, FName AbilityName);

	UFUNCTION(Server, Reliable)
	void ServerStopAbility(AActor* Instigator, FName AbilityName);
	
public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	static URAbilityComponent* GetAbilityComponent(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	URAbility* GetAbility(TSubclassOf<URAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void AddAbility(AActor* Instigator, TSubclassOf<URAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void RemoveAbility(URAbility* AbilityToRemove);
	
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool StartAbilityByName(AActor* Instigator, FName AbilityName);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool StopAbilityByName(AActor* Instigator, FName AbilityName);

	UPROPERTY(BlueprintAssignable, Category = "Abilities")
	FOnAbilityStateChanged OnAbilityStarted;

	UPROPERTY(BlueprintAssignable, Category = "Abilities")
	FOnAbilityStateChanged OnAbilityStopped;
	
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

};
