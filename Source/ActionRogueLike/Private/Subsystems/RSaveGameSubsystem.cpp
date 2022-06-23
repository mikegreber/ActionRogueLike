// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/RSaveGameSubsystem.h"

#include "EngineUtils.h"
#include "RPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Subsystems/RSaveGameSettings.h"

void URSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const URSaveGameSettings* SGSettings = GetDefault<URSaveGameSettings>();
	// set defaults from DefaultGame.ini
	SlotName = SGSettings->SaveSlotName;
}

void URSaveGameSubsystem::HandleStartingNewPlayer(AController* NewPlayer)
{
	ARPlayerState* PS = NewPlayer->GetPlayerState<ARPlayerState>();
	if (ensure(PS))
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
}

bool URSaveGameSubsystem::OverrideSpawnTransform(AController* NewPlayer)
{
	if (!IsValid(NewPlayer))
	{
		return false;
	}

	if (ARPlayerState* PS = NewPlayer->GetPlayerState<ARPlayerState>())
	{
		if (APawn* Pawn = PS->GetPawn())
		{
			if (FPlayerSaveData* SaveData = CurrentSaveGame->GetPlayerData(PS); SaveData && SaveData->bResumeAtTransform)
			{
				Pawn->SetActorLocation(SaveData->Location);
				Pawn->SetActorRotation(SaveData->Rotation);

				AController* PC = Cast<AController>(PS->GetOwner());
				PC->SetControlRotation(SaveData->Rotation);

				return true;
			}
		}
		
		return false;
	}

	
	return false;
}

void URSaveGameSubsystem::SetSlotName(FString NewSlotName)
{
	if (NewSlotName.Len() == 0)
	{
		return;
	}

	SlotName = NewSlotName;
}

void URSaveGameSubsystem::WriteSaveGame()
{
	CurrentSaveGame->SavedPlayers.Empty();
	CurrentSaveGame->SavedActors.Empty();

	AGameStateBase* GS = GetWorld()->GetGameState();
	if (GS == nullptr)
	{
		return;
	}

	for (TObjectPtr<APlayerState> PlayerState : GS->PlayerArray)
	{
		ARPlayerState* PS = Cast<ARPlayerState>(PlayerState);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break;
		}
	}
	
	// for (int32 i = 0; i < GS->PlayerArray.Num(); ++i)
	// {
	// 	ARPlayerState* PS = Cast<ARPlayerState>(GS->PlayerArray[i]);
	// 	if (PS)
	// 	{
	// 		PS->SavePlayerState(CurrentSaveGame);
	// 		break;
	// 	}
	// }
	
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!IsValid(Actor) || !Actor->Implements<URGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetFName();
		ActorData.Transform = Actor->GetActorTransform();

		// pass array to fill with data from Actor
		FMemoryWriter MemWriter(ActorData.ByteData);
		
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		// find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;
		// converts Actor's SaveGame UPROPERTIES into binary array
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);

	OnSaveGameWritten.Broadcast(CurrentSaveGame);
}

void URSaveGameSubsystem::LoadSaveGame(FString InSlotName)
{
	SetSlotName(InSlotName);

	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<URSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));

		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (!Actor->Implements<URGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetFName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;

					// convert binary back into actor's SaveGame UPROPERTY variables
					Actor->Serialize(Ar);

					IRGameplayInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}

		OnSaveGameLoaded.Broadcast(CurrentSaveGame);
	}
	else
	{
		CurrentSaveGame = Cast<URSaveGame>(UGameplayStatics::CreateSaveGameObject(URSaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}
}
