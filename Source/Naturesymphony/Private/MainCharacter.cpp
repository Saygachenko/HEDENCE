// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HealthComponent.h"
#include "GameFrameWork/Character.h"
#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"
#include "BaseWeaponActor.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("HealthComponent"));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (InputMapping)
			{
				InputSystem->AddMappingContext(InputMapping, 0);
			}
		}
	}

	LandedDelegate.AddDynamic(this, &AMainCharacter::OnGroundLanded);
	HealthComponent->OnDeath.AddDynamic(this, &AMainCharacter::OnDeath);

	SpawnWeapon();
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		Input->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
		Input->BindAction(JumpInputAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		Input->BindAction(WalkInputAction, ETriggerEvent::Started, this, &AMainCharacter::StartWalkMovement);
		Input->BindAction(WalkInputAction, ETriggerEvent::Completed, this, &AMainCharacter::StopWalkMovement);
		Input->BindAction(CrouchInputAction, ETriggerEvent::Triggered, this, &AMainCharacter::StartCrouchMovement);
		Input->BindAction(CrouchInputAction, ETriggerEvent::Completed, this, &AMainCharacter::StopCrouchMovement);
	}
}

// Function movement for character
void AMainCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D AxisValue = Value.Get<FVector2D>();

	if (Controller && AxisValue.Y != 0.f || AxisValue.X != 0.f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, AxisValue.Y);
		AddMovementInput(RightDirection, AxisValue.X);
	}
}

// Function rotation camera
void AMainCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D AxisValue = Value.Get<FVector2D>();

	AddControllerPitchInput(AxisValue.Y);
	AddControllerYawInput(AxisValue.X);
}

// Function started walk for character
void AMainCharacter::StartWalkMovement()
{
	if(GetCharacterMovement())
	{ 
		GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	}
}

// Function stoped walk for character
void AMainCharacter::StopWalkMovement()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
}

// Function started crouch for character
void AMainCharacter::StartCrouchMovement()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
		GetCharacterMovement()->GetNavAgentPropertiesRef().bCanJump = false;
	}
}

// Function stoped crouch for character 
void AMainCharacter::StopCrouchMovement()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = false;
		GetCharacterMovement()->GetNavAgentPropertiesRef().bCanJump = true;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
}

// Function falling on ground from height and take damage
void AMainCharacter::OnGroundLanded(const FHitResult& Hit)
{
	const float FallVelocityZ = -GetCharacterMovement()->Velocity.Z;
	UE_LOG(LogTemp, Display, TEXT("Falling from: %f"), FallVelocityZ);
	if (FallVelocityZ > MinVelocityZ)
	{
		TRange<float> LandedRangeVelocity(MinVelocityZ, MaxVelocityZ);
		TRange<float> LandedDamageRange(MinDamageLanded, MaxDamageLanded);
		const float FinalDamage = FMath::GetMappedRangeValueClamped(LandedRangeVelocity, LandedDamageRange, FallVelocityZ);
		UE_LOG(LogTemp, Display, TEXT("Final Damage: %f"), FinalDamage);
		TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
	}
}

// Function delegate death Character
void AMainCharacter::OnDeath()
{
	GetCharacterMovement()->DisableMovement();
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetLifeSpan(1000.0f);
}

// Function spawn weapon
void AMainCharacter::SpawnWeapon()
{
	if (GetWorld())
	{
		ABaseWeaponActor* Weapon = GetWorld()->SpawnActor<ABaseWeaponActor>(WeaponClass);
		if (Weapon)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(GetMesh(), AttachmentRules, "WeaponSocket");
		}
	}
}