// Fill out your copyright notice in the Description page of Project Settings.


#include "RTeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ARTeleportProjectile::ARTeleportProjectile()
{
	MovementComp->InitialSpeed = 5000.0f;

	SphereComp->OnComponentHit.AddDynamic(this, &ARTeleportProjectile::OnHit);
	SphereComp->InitSphereRadius(36);

	TeleportDelay = 0.2f;
	TeleportTime = 0.2f;
}

void ARTeleportProjectile::Teleport()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Teleport);
	
	MovementComp->StopMovementImmediately();
	EffectComp->DeactivateSystem();
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportEnterEffect, GetActorLocation());
	
	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ARTeleportProjectile::Teleport_TimeElapsed, TeleportDelay);
}

void ARTeleportProjectile::Teleport_TimeElapsed()
{
	FVector ProjectileLocation = GetActorLocation();
	AActor* ActorToTeleport = GetInstigator();
	
	ActorToTeleport->TeleportTo(ProjectileLocation, ActorToTeleport->GetActorRotation(), false, false);
	
	UGameplayStatics::SpawnEmitterAtLocation(this, TeleportCloseEffect, ProjectileLocation);

	Destroy();
}

void ARTeleportProjectile::OnHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, FVector Vector, const FHitResult& HitResult)
{
	Teleport();
}

void ARTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ARTeleportProjectile::Teleport, TeleportTime);
}
