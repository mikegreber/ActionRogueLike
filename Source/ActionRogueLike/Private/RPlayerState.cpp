// Fill out your copyright notice in the Description page of Project Settings.


#include "RPlayerState.h"
#include "Net/UnrealNetwork.h"

int32 ARPlayerState::GetCredits() const
{
	return Credits;
}

void ARPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta > 0))
	{
		return;	
	}

	int32 OldCredits = Credits;
	Credits += Delta;

	OnRep_Credits(OldCredits);
}

bool ARPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta > 0))
	{
		return false;	
	}
	
	if (Credits < Delta)
	{
		return false;
	}

	int32 OldCredits = Credits;

	Credits -= Delta;

	OnRep_Credits(OldCredits);
	
	return true;
}

void ARPlayerState::SavePlayerState_Implementation(URSaveGame* SaveObject)
{
	if (SaveObject)
	{
		FPlayerSaveData SaveData;
		SaveData.Credits = Credits;
		SaveData.PersonalRecordTime = PersonalRecordTime;
		SaveData.PlayerID = GetUniqueId().ToString();

		if (APawn* MyPawn = GetPawn())
		{
			SaveData.Location = MyPawn->GetActorLocation();
			SaveData.Rotation = MyPawn->GetActorRotation();
			SaveData.bResumeAtTransform = true;
		}

		SaveObject->SavedPlayers.Add(SaveData);
	}
}

void ARPlayerState::LoadPlayerState_Implementation(URSaveGame* SaveObject)
{
	if (SaveObject)
	{
		if (FPlayerSaveData* SaveData = SaveObject->GetPlayerData(this))
		{
			AddCredits(SaveData->Credits);

			PersonalRecordTime = SaveData->PersonalRecordTime;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadPlayerState called with nullptr."))
	}
}

void ARPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

void ARPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPlayerState, Credits);
}
