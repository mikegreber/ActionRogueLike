// Fill out your copyright notice in the Description page of Project Settings.


#include "RInteractionComponent.h"

#include "RCharacter.h"
#include "RGameplayInterface.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "UI/RWorldUserWidget.h"

static TAutoConsoleVariable CVarDebugDrawInteraction(TEXT("ARL.DebugDrawInteraction"), false, TEXT("Enable debug drawing for interaction component"), ECVF_Cheat);

// Sets default values for this component's properties
URInteractionComponent::URInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	TraceRadius = 30.0f;
	TraceDistance = 500.f;
	CollisionChannel = ECC_WorldDynamic;
}

void URInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();
}

void URInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	
	AActor* MyOwner = GetOwner();

	UCameraComponent* CameraComponent = Cast<ARCharacter>(MyOwner)->GetCameraComponent();
	
	// Line trace to find where camera is looking
	FVector TraceStart = CameraComponent->GetComponentLocation();
	FVector TraceEnd = TraceStart + CameraComponent->GetForwardVector() * TraceDistance;
	
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(CollisionChannel);

	TArray<FHitResult> Hits;
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	FocusedActor = nullptr;

	// iterate hit results for first hit implementing URGameplayInterface
	
	for (FHitResult& Hit : Hits)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<URGameplayInterface>())
			{
				if (bDebugDraw)
				{
					DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 2);
				}

				FocusedActor = HitActor;
				break;
			}
		}
	}

	// toggle UI widget

	if (FocusedActor)
	{
		if (DefaultWidget == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidget = CreateWidget<URWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidget)
		{
			DefaultWidget->AttachedActor = FocusedActor;

			if (!DefaultWidget->IsInViewport())
			{
				DefaultWidget->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidget)
		{
			DefaultWidget->RemoveFromParent();
		}
		
	}

	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, LineColor, false, 2, 0, 2);
	}
}


void URInteractionComponent::PrimaryInteract()
{
	if (FocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to Interact");
		return;
	}
	
	APawn* MyPawn = Cast<APawn>(GetOwner());
	
	IRGameplayInterface::Execute_Interact(FocusedActor, MyPawn);
}
