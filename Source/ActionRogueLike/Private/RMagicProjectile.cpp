// Fill out your copyright notice in the Description page of Project Settings.


#include "RMagicProjectile.h"

#include "RAttributeComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ARMagicProjectile::ARMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MovementComp->InitialSpeed = 1000.0f;
}

// Called when the game starts or when spawned
void ARMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ARMagicProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetActorLocation(), GetActorRotation());
		if (URAttributeComponent* AttributeComp = Cast<URAttributeComponent>(OtherActor->GetComponentByClass(URAttributeComponent::StaticClass())))
		{
			AttributeComp->ApplyHealthChange(-20);
		}
		Destroy();
	}
	
}

// Called every frame
void ARMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

