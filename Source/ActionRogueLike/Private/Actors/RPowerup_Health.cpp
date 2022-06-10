// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RPowerup_Health.h"

#include "RAttributeComponent.h"
#include "RPlayerState.h"

ARPowerup_Health::ARPowerup_Health()
{
	HealAmount = 50.f;
	CreditCost = 10;
}

void ARPowerup_Health::Interact_Implementation(APawn* InstigatorPawn)
{
	UE_LOG(LogTemp, Log, TEXT("INTERACT"))
	if (URAttributeComponent* AttributeComp = URAttributeComponent::GetAttributes(InstigatorPawn))
	{
		UE_LOG(LogTemp, Log, TEXT("HAS ATTRIBUTES"))
		if (ARPlayerState* PS = InstigatorPawn->GetPlayerState<ARPlayerState>())
		{
			UE_LOG(LogTemp, Log, TEXT("HAS PLAYERSTATE"))
			if (PS->GetCredits() >= CreditCost)
			{
				UE_LOG(LogTemp, Log, TEXT("INSUFICCIENT CREDITS"))

				if (AttributeComp->ApplyHealthChange(this, HealAmount))
				{
					UE_LOG(LogTemp, Log, TEXT("HEALTH GRANTED"))
					
					PS->RemoveCredits(CreditCost);
					Disable();
				}
			}
		}
	}
}
