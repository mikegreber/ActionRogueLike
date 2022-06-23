// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RSaveGameSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta = (DisplayName="Save Game Settings"))
class ACTIONROGUELIKE_API URSaveGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/* Default slot name if UI doesn't specify one */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	FString SaveSlotName;

	URSaveGameSettings();
};
