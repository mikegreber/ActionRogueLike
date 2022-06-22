// Fill out your copyright notice in the Description page of Project Settings.


#include "RGameModeBase.h"

#include "EngineUtils.h"
#include "RCharacter.h"
#include "RPlayerState.h"
#include "RSaveGame.h"
#include "Abilities/RAbilityComponent.h"
#include "ActionRogueLike/ActionRogueLike.h"
#include "AI/RAICharacter.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable CVarSpawnBots(TEXT("ARL.SpawnBots"), true, TEXT("Enable spawning of bots via timer"), ECVF_Cheat);


ARGameModeBase::ARGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	CreditsPerKill = 20;

	DesiredPowerupCount = 10;
	RequiredPowerupDistance = 2000;

	PlayerStateClass = ARPlayerState::StaticClass();

	SlotName = "SaveGame01";
}

void ARGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	if (SelectedSaveSlot.Len() > 0)
	{
		SlotName = SelectedSaveSlot;
	}
}

void ARGameModeBase::SpawnPowerups()
{
	if (ensure(!PowerupClasses.IsEmpty()))
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnPowerupsQuery, this, EEnvQueryRunMode::AllMatching, nullptr);

		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ARGameModeBase::OnSpawnPowerupsQueryComplete);
		}
	}

	
}

void ARGameModeBase::OnSpawnPowerupsQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn powerups EQS failed!"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	TArray<FVector> UsedLocations;
	
	int32 SpawnCounter = 0;
	while (SpawnCounter < DesiredPowerupCount && Locations.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, Locations.Num() - 1);
		FVector PickedLocation = Locations[RandomIndex];
		Locations.RemoveAt(RandomIndex);
		bool bIsValidLocation = true;

		for (FVector Location : UsedLocations)
		{
			float Distance = (PickedLocation - Location).Size();

			if (Distance < RequiredPowerupDistance)
			{
				bIsValidLocation = false;
				break;
			}
		}

		if (bIsValidLocation)
		{
			int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
			
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			GetWorld()->SpawnActor<AActor>(PowerupClasses[RandomClassIndex], PickedLocation + FVector(0.f,0.f, 100.0f), FRotator::ZeroRotator, Params);
			
			++SpawnCounter;
		}
	}
}

void ARGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ARGameModeBase::SpawnBot, SpawnTimerInterval, true);

	FTimerHandle TimerHandle_SpawnPickups;
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnPickups, this, &ARGameModeBase::SpawnPowerups, SpawnTimerInterval, false);

	LoadSaveGame();
}

void ARGameModeBase::SpawnBot()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Log, TEXT("Bot spawning disabled"));
		return;
	}
	
	int NumAliveBots = 0;
	for (TActorIterator<ARAICharacter> It(GetWorld()); It; ++It)
	{
		ARAICharacter* Bot = *It;
		URAttributeComponent* AttributeComp = URAttributeComponent::GetAttributes(Bot);
		if (AttributeComp && AttributeComp->IsAlive())
		{
			++NumAliveBots;
		}
	}

	float MaxBotCount = 10;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NumAliveBots >= MaxBotCount)
	{
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ARGameModeBase::OnSpawnBotsQueryCompleted);
	}
}



void ARGameModeBase::OnSpawnBotsQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS failed!"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsValidIndex(0))
	{
		if (EnemyTable)
		{
			TArray<FEnemyInfoRow*> Rows;
			EnemyTable->GetAllRows("", Rows);

			// Get random enemy
			int32 RandomIndex = FMath::RandRange(0, Rows.Num()-1);
			FEnemyInfoRow* SelectedRow = Rows[RandomIndex];

			UAssetManager* Manager = UAssetManager::GetIfValid();
			if (Manager)
			{
				LogOnScreen(this, "Loading enemy...", FColor::Green);
				
				TArray<FName> Bundles;
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ARGameModeBase::OnEnemyLoaded, SelectedRow->EnemyId, Locations[0]);
				Manager->LoadPrimaryAsset(SelectedRow->EnemyId, Bundles, Delegate);
			}
			
			
		}
	}
}

void ARGameModeBase::OnEnemyLoaded(FPrimaryAssetId PrimaryAssetId, FVector SpawnLocation)
{
	LogOnScreen(this, "Finished loading enemy.", FColor::Green);
	
	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		UREnemyData* EnemyData = Cast<UREnemyData>(Manager->GetPrimaryAssetObject(PrimaryAssetId));
		if (EnemyData)
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
			AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(EnemyData->EnemyClass, SpawnLocation + FVector(0.f,0.f, 100.0f), FRotator::ZeroRotator, Params);
			URAbilityComponent* AbilityComp = URAbilityComponent::GetAbilityComponent(SpawnedActor);
			if (AbilityComp)
			{
				for (TSubclassOf<URAbility> Ability : EnemyData->Abilities)
				{
					AbilityComp->AddAbility(SpawnedActor, Ability);
				}
			}
		}
		else
		{
			LogOnScreen(this, "Enemy data null.", FColor::Red);
		}
	}
	else
	{
		LogOnScreen(this, "Manager null", FColor::Red);
	}
}

void ARGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		
		RestartPlayer(Controller);
	}
}

void ARGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	// respawn player after delay
	ARCharacter* Player = Cast<ARCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	// give credits for kill
	if (APawn* KillerPawn = Cast<APawn>(Killer))
	{
		if (ARPlayerState* KillerPlayerState = KillerPawn->GetPlayerState<ARPlayerState>())
		{
			KillerPlayerState->AddCredits(CreditsPerKill);
		}
	}
	

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
	
}

void ARGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (ARPlayerState* PS = NewPlayer->GetPlayerState<ARPlayerState>())
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void ARGameModeBase::KillAll()
{
	for (TActorIterator<ARAICharacter> It(GetWorld()); It; ++It)
	{
		ARAICharacter* Bot = *It;
		URAttributeComponent* AttributeComp = URAttributeComponent::GetAttributes(Bot);
		if (AttributeComp && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this);
		}
	}
}

void ARGameModeBase::WriteSaveGame()
{
	for (int32 i = 0; i < GameState->PlayerArray.Num(); ++i)
	{
		ARPlayerState* PS = Cast<ARPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break;
		}
	}

	CurrentSaveGame->SavedActors.Empty();
	
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<URGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();

		// pass array to fill with data from actor
		FMemoryWriter MemWriter(ActorData.ByteData);

		
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		Ar.ArIsSaveGame = true; // find only variables with UPROPERTY(SaveGame)

		// converts actors SaveGame UPROPERTIES into binary array
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}
	
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ARGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<URSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."))

		for (FActorIterator It(GetWorld()); It; ++It)
		{
			
			AActor* Actor = *It;
			if (!Actor->Implements<URGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					// pass array to read data from actor
					FMemoryReader MemReader(ActorData.ByteData);
					
					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true; // find only variables with UPROPERTY(SaveGame)

					// converts actors binary array back into actors variables
					Actor->Serialize(Ar);

					IRGameplayInterface::Execute_OnActorLoaded(Actor);
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<URSaveGame>(UGameplayStatics::CreateSaveGameObject(URSaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Created new SaveGame Data."))
	}
}
