// Fill out your copyright notice in the Description page of Project Settings.


#include "RSaveGame.h"

#include "GameFramework/PlayerState.h"

FPlayerSaveData* URSaveGame::GetPlayerData(APlayerState* PlayerState)
{
	if (PlayerState == nullptr)
	{
		return nullptr;
	}

	if (PlayerState->GetWorld()->IsPlayInEditor())
	{
		UE_LOG(LogTemp, Log, TEXT("During PIE we cannot use PlayerID to retrieve Saved Player data. Using first entry in array if available."));

		if (SavedPlayers.IsValidIndex(0))
		{
			return &SavedPlayers[0];
		}

		return nullptr;
	}
	
	FString PlayerID = PlayerState->GetUniqueId().ToString();

	return SavedPlayers.FindByPredicate([&](const FPlayerSaveData& Data) { return Data.PlayerID == PlayerID; });
}
