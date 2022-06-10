// Fill out your copyright notice in the Description page of Project Settings.


#include "RBlackholeProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ARBlackholeProjectile::ARBlackholeProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComp->SetGenerateOverlapEvents(true);
	SphereComp->SetNotifyRigidBodyCollision(false);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ARBlackholeProjectile::OnSphereComponentBeginOverlap);
	
	MovementComp->InitialSpeed = 50.0f;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->ForceStrength = -10000;
	ForceComp->SetupAttachment(RootComponent);

	InitialLifeSpan = 5;
}

void ARBlackholeProjectile::OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
	
}

