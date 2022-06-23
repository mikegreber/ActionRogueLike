// Fill out your copyright notice in the Description page of Project Settings.


#include "RGameModeBase.h"

#include "EngineUtils.h"
#include "RCharacter.h"
#include "RPlayerState.h"
#include "Abilities/RAbilityComponent.h"
#include "ActionRogueLike/ActionRogueLike.h"
#include "AI/RAICharacter.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/RSaveGameSubsystem.h"

static TAutoConsoleVariable CVarSpawnBots(TEXT("ARL.SpawnBots"), true, TEXT("Enable spawning of bots via timer"), ECVF_Cheat);

ARGameModeBase::ARGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	CreditsPerKill = 20;

	DesiredPowerupCount = 10;
	RequiredPowerupDistance = 2000;

	PlayerStateClass = ARPlayerState::StaticClass();
}

void ARGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	URSaveGameSubsystem* SG = GetGameInstance()->GetSubsystem<URSaveGameSubsystem>();

	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	SG->LoadSaveGame(SelectedSaveSlot);
}

void ARGameModeBase::StartPlay()
{
	Super::StartPlay();

	SpawnPowerups();
	
	// continuous timer to spawn bots
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ARGameModeBase::SpawnBot, SpawnTimerInterval, true);
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
				TArray<FName> Bundles;
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ARGameModeBase::OnEnemyLoaded, SelectedRow->EnemyId, Locations[0]);
				Manager->LoadPrimaryAsset(SelectedRow->EnemyId, Bundles, Delegate);
			}
			
			
		}
	}
}

void ARGameModeBase::OnEnemyLoaded(FPrimaryAssetId PrimaryAssetId, FVector SpawnLocation)
{
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
	// call before Super:: so we set variables before beginplayingstate is called in PlayerController (for UI instantiation)
	URSaveGameSubsystem* SG = GetGameInstance()->GetSubsystem<URSaveGameSubsystem>();
	SG->HandleStartingNewPlayer(NewPlayer);

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	
	SG->OverrideSpawnTransform(NewPlayer);
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
