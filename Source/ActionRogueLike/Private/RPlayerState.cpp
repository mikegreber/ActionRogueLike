// Fill out your copyright notice in the Description page of Project Settings.


#include "RPlayerState.h"

#include "ActionRogueLike/ActionRogueLike.h"
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
		UE_LOG(LogTemp, Log, TEXT("SavePlayerState"))
		SaveObject->Credits = Credits;
	}
}

void ARPlayerState::LoadPlayerState_Implementation(URSaveGame* SaveObject)
{
	if (SaveObject)
	{
		UE_LOG(LogTemp, Log, TEXT("LoadPlayerState"))
		AddCredits(SaveObject->Credits);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadPlayerState called with nullptr."))
	}
}

void ARPlayerState::OnRep_Credits(int32 OldCredits)
{
	LogOnScreen(this, TEXT("OnRep_Credits"), FColor::Blue, 5.0f);
	OnCreditsChanged.Broadcast(this, Credits);
}

void ARPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPlayerState, Credits);
}
