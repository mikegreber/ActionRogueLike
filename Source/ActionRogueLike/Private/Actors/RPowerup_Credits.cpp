// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RPowerup_Credits.h"

#include "RAttributeComponent.h"
#include "RPlayerState.h"

ARPowerup_Credits::ARPowerup_Credits()
{
	CreditAmount = 10;
	CooldownTime = 30.f;
}

void ARPowerup_Credits::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	UE_LOG(LogTemp, Warning, TEXT("INTERACT"))
	
	if (ARPlayerState* PS = InstigatorPawn->GetPlayerState<ARPlayerState>())
	{
		UE_LOG(LogTemp, Warning, TEXT("ADDING CREDITS"))
		PS->AddCredits(CreditAmount);
		Disable();
	}
}
