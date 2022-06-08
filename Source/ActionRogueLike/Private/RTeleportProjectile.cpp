// Fill out your copyright notice in the Description page of Project Settings.


#include "RTeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ARTeleportProjectile::ARTeleportProjectile()
{
	MovementComp->InitialSpeed = 1000.0f;

	SphereComp->OnComponentHit.AddDynamic(this, &ARTeleportProjectile::OnHit);
	SphereComp->InitSphereRadius(36);
}

void ARTeleportProjectile::Teleport()
{
	MovementComp->SetVelocityInLocalSpace(FVector::Zero());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportEnterEffect, GetActorLocation());

	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &ARTeleportProjectile::Teleport_TimeElapsed, 0.2f);
	
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void ARTeleportProjectile::Teleport_TimeElapsed()
{
	FVector ProjectileLocation = GetActorLocation();
	
	GetInstigator()->SetActorLocation(ProjectileLocation);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportCloseEffect, ProjectileLocation);

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
	
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ARTeleportProjectile::Teleport, TeleportTime);
}
