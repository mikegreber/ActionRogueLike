// Fill out your copyright notice in the Description page of Project Settings.


#include "RPlayerState.h"

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
	
	Credits += Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);
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

	Credits -= Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);
	
	return true;
}
