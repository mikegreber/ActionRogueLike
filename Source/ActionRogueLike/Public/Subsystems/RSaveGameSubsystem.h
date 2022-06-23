// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RSaveGameSubsystem.generated.h"

class URSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveGame, URSaveGame*, SaveObject);

UCLASS(meta = (DisplayName="SaveGame Subsystem"))
class ACTIONROGUELIKE_API URSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "SaveGame")
	FString SlotName;

	UPROPERTY()
	URSaveGame* CurrentSaveGame;

public:

	void Initialize(FSubsystemCollectionBase& Collection) override;

	void HandleStartingNewPlayer(AController* NewPlayer);

	UFUNCTION(BlueprintCallable)
	bool OverrideSpawnTransform(AController* NewPlayer);

	UFUNCTION(BlueprintCallable)
	void SetSlotName(FString NewSlotName);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame(FString InSlotName);

	UPROPERTY(BlueprintAssignable)
	FOnSaveGame OnSaveGameLoaded;
	
	UPROPERTY(BlueprintAssignable)
	FOnSaveGame OnSaveGameWritten;
};
