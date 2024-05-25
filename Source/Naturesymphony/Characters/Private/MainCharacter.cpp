// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Characters/Public/MainCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Naturesymphony/Components/Public/HealthComponent.h"
#include "GameFrameWork/Character.h"
#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Naturesymphony/Components/Public/InventorySystemComponent.h"
#include "Naturesymphony/Inventory/Items/Effects/Weapons/Public/BaseWeapon.h"
#include "Naturesymphony/Components/Public/CombatComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("Health"));
	InventorySystemComponent = CreateDefaultSubobject<UInventorySystemComponent>(FName("Inventory"));

	CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>(FName("CameraCollision"));
	CameraCollisionComponent->SetupAttachment(CameraComponent);

	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	SpringArmComponent->SocketOffset = FVector(50.0f, 50.0f, 30.0f);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 15.0f;

	CameraCollisionComponent->SetSphereRadius(10.0f);
	CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(FName("CombatComponent"));
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
	CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnCameraCollisionBeginOverlap);
	CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::OnCameraCollisionEndOverlap);
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
		Input->BindAction(JumpInputAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		Input->BindAction(WalkInputAction, ETriggerEvent::Started, this, &AMainCharacter::StartWalkMovement);
		Input->BindAction(WalkInputAction, ETriggerEvent::Completed, this, &AMainCharacter::StopWalkMovement);
		Input->BindAction(CrouchInputAction, ETriggerEvent::Started, this, &AMainCharacter::Crouch, false);
		Input->BindAction(CrouchInputAction, ETriggerEvent::Completed, this, &AMainCharacter::UnCrouch, false);
		Input->BindAction(InteractInputAction, ETriggerEvent::Started, InventorySystemComponent, &UInventorySystemComponent::Interact);
		Input->BindAction(EquipWeaponInputAction, ETriggerEvent::Started, this, &AMainCharacter::OnEquipedItem);
		Input->BindAction(LightAttackInputAction, ETriggerEvent::Started, this, &AMainCharacter::LightAttack);
		Input->BindAction(DodgeInputAction, ETriggerEvent::Started, this, &AMainCharacter::Dodge);
	}
}

// Function movement for character
void AMainCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D AxisValue = Value.Get<FVector2D>();

	if (Controller && AxisValue.Y != 0.0f || AxisValue.X != 0.0f)
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
void AMainCharacter::Crouch(bool bClientSimulation)
{
	if (!GetCharacterMovement()->IsFalling())
	{
		Super::Crouch(bClientSimulation);
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
	if (GetCharacterMovement() && GetMesh() && GetCapsuleComponent())
	{
		GetCharacterMovement()->DisableMovement();
		GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->DestroyComponent();
		SetLifeSpan(5.0f);
	}
}

// Function delegate OnComponentBeginOverlap
void AMainCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckCameraOverlap();
}

// Function delegate OnComponentEndOverlap
void AMainCharacter::OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckCameraOverlap();
}

// Function checked overlap camer and capsule of character
void AMainCharacter::CheckCameraOverlap()
{
	const bool HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
	GetMesh()->SetOwnerNoSee(HideMesh);
}

// Function checks the conditions of the variables for weapon availability and weapon-specific combat type, and triggers the animation when the button is pressed.
void AMainCharacter::OnEquipedItem()
{
	if (CombatComponent)
	{
		ABaseWeapon* MainWeapon = CombatComponent->GetMainWeapon();
		if (MainWeapon)
		{
			if (!CombatComponent->GetCombatEnabled())
			{
				PlayAnimMontage(MainWeapon->EnterCombat);
			}
			else
			{
				PlayAnimMontage(MainWeapon->ExitCombat);
			}
		}
	}
}

void AMainCharacter::PerformAttack(int32 AttackIndex, bool bRandomIndex)
{
	if (CombatComponent)
	{
		ABaseWeapon* MainWeapon = CombatComponent->GetMainWeapon();
		if (MainWeapon && MainWeapon->GetIsEquippedToHand())
		{
			UAnimMontage* AttackMontage = nullptr;

			if (bRandomIndex)
			{
				AttackIndex = FMath::RandRange(0, MainWeapon->AttackMontageArray.Num() - 1);
				AttackMontage = MainWeapon->AttackMontageArray[AttackIndex];
			}
			else
			{
				AttackMontage = MainWeapon->AttackMontageArray[AttackIndex];
			}

			if(AttackMontage)
			{
				CombatComponent->SetIsAttaking(true);
				PlayAnimMontage(AttackMontage);

				CombatComponent->AttackCount++;
				if (CombatComponent->AttackCount >= MainWeapon->AttackMontageArray.Num())
				{
					CombatComponent->AttackCount = 0;
				}
			}
		}
	}
}

// Function for inputting a weapon attack to the LKM button
void AMainCharacter::LightAttack()
{
	if (CombatComponent)
	{
		if (CombatComponent->GetIsAttaking())
		{
			CombatComponent->SetIsAttackSaved(true);
		}
		else
		{
			PerformAttack(CombatComponent->AttackCount, false);
		}
	}
}

void AMainCharacter::ContinueAttack_Implementation()
{
	if (CombatComponent)
	{
		CombatComponent->SetIsAttaking(false);

		if (CombatComponent->GetIsAttackSaved())
		{
			CombatComponent->SetIsAttackSaved(false);
		}
	}
}

void AMainCharacter::ResetAttack_Implementation()
{
	if (CombatComponent)
	{
		CombatComponent->ResetAttack();
		bIsDodging = false;
	}
}

void AMainCharacter::PerformDodge(int32 DodgeIndex, bool bRandomIndex)
{
	ABaseWeapon* MainWeapon = CombatComponent->GetMainWeapon();
	if (MainWeapon)
	{
		UAnimMontage* DodgeMontage = nullptr;

		if (bRandomIndex)
		{
			DodgeIndex = FMath::RandRange(0, MainWeapon->DodgeMontageArray.Num() - 1);
			DodgeMontage = MainWeapon->DodgeMontageArray[DodgeIndex];
		}
		else
		{
			DodgeMontage = MainWeapon->DodgeMontageArray[DodgeIndex];
		}

		if (DodgeMontage)
		{
			bIsDodging = true;
			PlayAnimMontage(DodgeMontage);

			MainWeapon->DodgeCount++;
			if (MainWeapon->DodgeCount >= MainWeapon->DodgeMontageArray.Num())
			{
				MainWeapon->DodgeCount = 0;
			}
		}
	}
}

// Function for inputtings a Character dodge on the SPACE button
void AMainCharacter::Dodge()
{
	if (CombatComponent)
	{
		ABaseWeapon* MainWeapon = CombatComponent->GetMainWeapon();
		if (MainWeapon)
		{
			if (!CombatComponent->GetIsAttaking() && !bIsDodging)
			{
				PerformDodge(MainWeapon->DodgeCount, false);
			}
		}
	}
}

FRotator AMainCharacter::GetDesiredRotation_Implementation()
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent)
	{
		FVector LastInputVector = CharacterMovementComponent->GetLastInputVector();

		bool bIsNotEqual = UKismetMathLibrary::EqualEqual_VectorVector(LastInputVector, FVector(0.0f, 0.0f, 0.0f), 0.001f);
		if (!bIsNotEqual)
		{
			return FRotator(GetLastMovementInputVector().Rotation());
		}
		else
		{
			return GetActorRotation();
		}
	}

	return FRotator();
}