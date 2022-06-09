// Fill out your copyright notice in the Description page of Project Settings.


#include "RInteractionComponent.h"

#include "RCharacter.h"
#include "RGameplayInterface.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
URInteractionComponent::URInteractionComponent()
{
}


void URInteractionComponent::PrimaryInteract()
{
	AActor* MyOwner = GetOwner();

	UCameraComponent* CameraComponent = Cast<ARCharacter>(MyOwner)->GetCameraComponent();
	
	// Line trace to find where camera is looking
	FVector TraceStart = CameraComponent->GetComponentLocation();
	FVector TraceEnd = TraceStart + CameraComponent->GetForwardVector() * 400;

	float Radius = 30.f;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjParams.AddObjectTypesToQuery(ECC_Pawn);

	TArray<FHitResult> Hits;
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	for (FHitResult& Hit : Hits)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<URGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
			
				IRGameplayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}
		
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2);
	}
	
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, LineColor, false, 2, 0, 2);
}
