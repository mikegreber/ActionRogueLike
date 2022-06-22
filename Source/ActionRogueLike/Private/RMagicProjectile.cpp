// Fill out your copyright notice in the Description page of Project Settings.


#include "RMagicProjectile.h"

#include "RAttributeComponent.h"
#include "RGameplayFunctionLibrary.h"
#include "Abilities/RAbilityComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ARMagicProjectile::ARMagicProjectile()
{
 	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ARMagicProjectile::OnActorOverlap);
	SphereComp->SetGenerateOverlapEvents(true);
	SphereComp->SetNotifyRigidBodyCollision(false);
	
	MovementComp->InitialSpeed = 8000.0f;

	DamageAmount = 30.0f;

	InitialLifeSpan = 5.0f;

	bReplicates = true;
}

void ARMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		// parry
		URAbilityComponent* AbilityComp = URAbilityComponent::GetAbilityComponent(OtherActor);
		if (AbilityComp && AbilityComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			SphereComp->ClearMoveIgnoreActors();
			SetInstigator(Cast<APawn>(OtherActor));
			
			MovementComp->Velocity = -MovementComp->Velocity;
			return;
		}
		
		if (URGameplayFunctionLibrary::ApplyDamageDirectional(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			if (AbilityComp && HasAuthority())
			{
				AbilityComp->AddAbility(GetInstigator(), BurningEffectClass);
			}
		}
		Explode();
	}
}


