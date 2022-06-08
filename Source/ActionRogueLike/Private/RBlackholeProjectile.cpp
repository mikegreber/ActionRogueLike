// Fill out your copyright notice in the Description page of Project Settings.


#include "RBlackholeProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ARBlackholeProjectile::ARBlackholeProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	// SphereComp->SetCollisionProfileName("Projectile");
	// SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ARBlackholeProjectile::OnBeginOverlap);
	// RootComponent = SphereComp;
	//
	// EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	// EffectComp->SetupAttachment(RootComponent);
	//
	// MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 50.0f;
	// MovementComp->bRotationFollowsVelocity = true;
	// MovementComp->bInitialVelocityInLocalSpace = true;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->ForceStrength = -10000;
	ForceComp->SetupAttachment(RootComponent);

	InitialLifeSpan = 5;
}


void ARBlackholeProjectile::OnBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult)
{
	Actor->Destroy();
}

// Called when the game starts or when spawned
void ARBlackholeProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

