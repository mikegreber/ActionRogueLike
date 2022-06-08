// Fill out your copyright notice in the Description page of Project Settings.


#include "RCharacter.h"

#include "RAttributeComponent.h"
#include "RInteractionComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


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

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
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
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ARCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ARCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ARCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("TeleportAbility", IE_Pressed, this, &ARCharacter::TeleportAbility);
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

void ARCharacter::SpawnProjectile(UClass* ProjectileClass)
{
	FVector CameraStart = CameraComponent->GetComponentLocation();
	FVector End = CameraStart + CameraComponent->GetForwardVector() * 5'000;

	if (FHitResult Hit; GetWorld()->LineTraceSingleByChannel(Hit, CameraStart, End, ECC_Visibility))
	{
		End = Hit.Location;
	}
	
	FVector ProjectileStart = GetMesh()->GetSocketLocation("Muzzle_01");
	FRotator Rotation = (End - ProjectileStart).Rotation();
	FTransform SpawnTF = FTransform(Rotation,ProjectileStart);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTF, SpawnParams);
}

void ARCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ARCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void ARCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(PrimaryProjectileClass);
}

void ARCharacter::SecondaryAttack()
{
	PlayAnimMontage(AttackAnim);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ARCharacter::SecondaryAttack_TimeElapsed, 0.2f);
}

void ARCharacter::SecondaryAttack_TimeElapsed()
{
	SpawnProjectile(SecondaryProjectileClass);
}

void ARCharacter::TeleportAbility()
{
	PlayAnimMontage(AttackAnim);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ARCharacter::TeleportAbility_TimeElapsed, 0.2f);
}

void ARCharacter::TeleportAbility_TimeElapsed()
{
	SpawnProjectile(TeleportProjectileClass);
}


void ARCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}
