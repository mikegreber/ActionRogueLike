// Fill out your copyright notice in the Description page of Project Settings.


#include "RCharacter.h"

#include "RAttributeComponent.h"
#include "RInteractionComponent.h"
#include "Abilities/RAbilityComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARCharacter::ARCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComponent->SetupAttachment(SpringArmComponent);

	InteractionComp = CreateDefaultSubobject<URInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<URAttributeComponent>("AttributeComp");

	AbilityComp = CreateDefaultSubobject<URAbilityComponent>("AbilityComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	TimeToHitParamName = "TimeToHit";
}

void ARCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ARCharacter::OnHealthChanged);
}

FVector ARCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
}

// Called when the game starts or when spawned
void ARCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void ARCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



// Called to bind functionality to input
void ARCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ARCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ARCharacter::SprintStop);
	
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ARCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ARCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ARCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("TeleportAbility", IE_Pressed, this, &ARCharacter::DashAbility);
	
}



void ARCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;
	
	AddMovementInput(ControlRot.Vector(), Value);
}

void ARCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;
	
	AddMovementInput(FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y), Value);
}

void ARCharacter::SprintStart()
{
	AbilityComp->StartAbilityByName(this, "Sprint");
}

void ARCharacter::SprintStop()
{
	AbilityComp->StopAbilityByName(this, "Sprint");
}

void ARCharacter::PrimaryAttack()
{
	AbilityComp->StartAbilityByName(this, "PrimaryAttack");
}

void ARCharacter::SecondaryAttack()
{
	AbilityComp->StartAbilityByName(this, "Blackhole");
}

void ARCharacter::DashAbility()
{
	AbilityComp->StartAbilityByName(this, "Dash");
}

void ARCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ARCharacter::OnHealthChanged(AActor* InstigatorActor, URAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->GetTimeSeconds());

		if (NewHealth <= 0.0f)
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			DisableInput(PC);

			SetLifeSpan(5.0f);
		}
	}
}


void ARCharacter::HealSelf(float Amount)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}