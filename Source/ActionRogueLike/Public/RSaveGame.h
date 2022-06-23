// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RSaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FName ActorName;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	TArray<uint8> ByteData;
};

USTRUCT()
struct FPlayerSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FString PlayerID;

	UPROPERTY()
	int32 Credits;

	UPROPERTY()
	float PersonalRecordTime;

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;

	UPROPERTY()
	bool bResumeAtTransform;
};


/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FPlayerSaveData> SavedPlayers;
	
	UPROPERTY()
	TArray<FActorSaveData> SavedActors;

	FPlayerSaveData* GetPlayerData(APlayerState* PlayerState);
};
