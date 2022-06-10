// Fill out your copyright notice in the Description page of Project Settings.


#include "RGameplayFunctionLibrary.h"

#include "RAttributeComponent.h"

bool URGameplayFunctionLibrary::ApplyDamage(AActor* InstigatorActor, AActor* TargetActor, float DamageAmount)
{
	URAttributeComponent* AttributeComp = URAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(InstigatorActor, -DamageAmount);
	}

	return false;
}

bool URGameplayFunctionLibrary::ApplyDamageDirectional(AActor* InstigatorActor, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(InstigatorActor, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();
			
			HitComp->AddImpulseAtLocation(Direction * 300000.f, HitResult.Location, HitResult.BoneName);
		}
		return true;
	}

	return false;
}
