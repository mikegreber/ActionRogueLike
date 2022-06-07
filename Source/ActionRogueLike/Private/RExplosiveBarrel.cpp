// Fill out your copyright notice in the Description page of Project Settings.


#include "RExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ARExplosiveBarrel::ARExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	
	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->ForceStrength = 1000;
	ForceComp->SetupAttachment(RootComponent);

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ARExplosiveBarrel::OnHit);
}

void ARExplosiveBarrel::OnHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT("HIT!!"))
	ForceComp->FireImpulse();
}

// Called when the game starts or when spawned
void ARExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

