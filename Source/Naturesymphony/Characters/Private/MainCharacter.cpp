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
#include "StateManagerComponent.h"
#include "CharacterState.h"

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

	StateManagerComponent = CreateDefaultSubobject<UStateManagerComponent>(FName("StateManagerComponent"));
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
	StateManagerComponent->OnStateBegin.AddDynamic(this, &AMainCharacter::OnStateBegin);
	StateManagerComponent->OnStateEnd.AddDynamic(this, &AMainCharacter::OnStateEnd);
	StateManagerComponent->OnCharacterActionBegin.AddDynamic(this, &AMainCharacter::OnActionBegin);
	StateManagerComponent->OnCharacterActionEnd.AddDynamic(this, &AMainCharacter::OnActionEnd);
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
		Input->BindAction(JumpInputAction, ETriggerEvent::Started, this, &AMainCharacter::Jump);
		Input->BindAction(JumpInputAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		Input->BindAction(WalkInputAction, ETriggerEvent::Started, this, &AMainCharacter::WalkInput);
		Input->BindAction(WalkInputAction, ETriggerEvent::Completed, this, &AMainCharacter::StopWalkInput);
		Input->BindAction(CrouchInputAction, ETriggerEvent::Started, this, &AMainCharacter::Crouch, false);
		Input->BindAction(CrouchInputAction, ETriggerEvent::Completed, this, &ACharacter::UnCrouch, false);
		Input->BindAction(InteractInputAction, ETriggerEvent::Started, InventorySystemComponent, &UInventorySystemComponent::Interact);
		Input->BindAction(EquipWeaponInputAction, ETriggerEvent::Started, this, &AMainCharacter::EquipInput);
		Input->BindAction(LightAttackInputAction, ETriggerEvent::Started, this, &AMainCharacter::AttackInput);
		Input->BindAction(DodgeInputAction, ETriggerEvent::Started, this, &AMainCharacter::DodgeInput);
		Input->BindAction(SprintInputAction, ETriggerEvent::Started, this, &AMainCharacter::SprintInput);
		Input->BindAction(SprintInputAction, ETriggerEvent::Completed, this, &AMainCharacter::StopSprintInput);
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
void AMainCharacter::WalkInput()
{
	SetMovementSpeedMode(EMovementSpeedMode::Walking);
}

// Function stoped walk for character
void AMainCharacter::StopWalkInput()
{
	if (GetCurrentMovementSpeedMode() == EMovementSpeedMode::Walking)
	{
		SetMovementSpeedMode(EMovementSpeedMode::Jogging);
	}
}

// Function started crouch for character
void AMainCharacter::Crouch(bool bClientSimulation)
{
	if (CanPerformCrouch())
	{
		if (StateManagerComponent && CombatComponent)
		{
			StopAnimMontage();
			StateManagerComponent->ResetState();
			CombatComponent->ResetAttack();

			Super::Crouch(bClientSimulation);
		}
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
	UCharacterMovementComponent* CharactermMovementComponent = GetCharacterMovement();
	if (CharactermMovementComponent)
	{	
		USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
		if (SkeletalMeshComponent)
		{
			UCapsuleComponent* MainCapsuleComponent = GetCapsuleComponent();
			if (MainCapsuleComponent)
			{
				if (SpringArmComponent)
				{
					CharactermMovementComponent->DisableMovement();
					SkeletalMeshComponent->SetSimulatePhysics(true);
					MainCapsuleComponent->DestroyComponent();
					SpringArmComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform);

					SetLifeSpan(5.0f);
				}
			}
		}
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
void AMainCharacter::EquipInput()
{
	if (CanPerformEquip())
	{
		if (StateManagerComponent)
		{
			StateManagerComponent->SetState(ECharacterState::Equipping);

			if (CombatComponent)
			{
				ABaseWeapon* MainWeapon = CombatComponent->GetMainWeapon();
				if (MainWeapon)
				{
					if (!CombatComponent->GetCombatEnabled())
					{
						TArray<UAnimMontage*> ActionMontage = { MainWeapon->EnterCombat };
						PerformAction(ActionMontage, ECharacterAction::EnterCombat);
					}
					else
					{
						TArray<UAnimMontage*> ActionMontage = { MainWeapon->ExitCombat };
						PerformAction(ActionMontage, ECharacterAction::ExitCombat);
					}
				}
			}
		}
	}
}

void AMainCharacter::PerformAttack(ECharacterAction AttackType, int32 AttackIndex, bool bRandomIndex)
{
	if (CombatComponent)
	{
		ABaseWeapon* MainWeapon = CombatComponent->GetMainWeapon();
		if (MainWeapon && CombatComponent->GetCombatEnabled())
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
				StateManagerComponent->SetState(ECharacterState::Attacking);
				StateManagerComponent->SetCurrentAction(AttackType);

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
void AMainCharacter::AttackInput()
{
	if (StateManagerComponent)
	{
		if (CombatComponent)
		{
			if (StateManagerComponent->GetCurrentState() == ECharacterState::Attacking)
			{
				CombatComponent->SetIsAttackSaved(true);
			}
			else
			{
				Attack();
			}
		}
	}
}

void AMainCharacter::ContinueAttack_Implementation()
{
	if (CombatComponent)
	{
		if (CombatComponent->GetIsAttackSaved())
		{
			CombatComponent->SetIsAttackSaved(false);

			if (StateManagerComponent->GetCurrentState() == ECharacterState::Attacking)
			{
				StateManagerComponent->SetState(ECharacterState::None);
			}

			Attack();
		}
	}
}

void AMainCharacter::ResetAttack_Implementation()
{
	if (CombatComponent)
	{
		if (StateManagerComponent)
		{
			CombatComponent->ResetAttack();
			StateManagerComponent->ResetState();
			StateManagerComponent->SetCurrentAction(ECharacterAction::None);
		}
	}
}

void AMainCharacter::PerformDodge(int32 DodgeIndex, bool bRandomIndex)
{
	if (!DodgeMontageArray.IsEmpty())
	{
		UAnimMontage* DodgeMontage = nullptr;

		if (bRandomIndex)
		{
			DodgeIndex = FMath::RandRange(0, DodgeMontageArray.Num() - 1);
			DodgeMontage = DodgeMontageArray[DodgeIndex];
		}
		else
		{
			DodgeMontage = DodgeMontageArray[DodgeIndex];
		}

		if (DodgeMontage)
		{
			if (StateManagerComponent)
			{
				StateManagerComponent->SetState(ECharacterState::Dodging);
				StateManagerComponent->SetCurrentAction(ECharacterAction::Dodge);

				PlayAnimMontage(DodgeMontage);

				DodgeCount++;
				if (DodgeCount >= DodgeMontageArray.Num())
				{
					DodgeCount = 0;
				}
			}
		}
	}
}

// Function for inputtings a Character dodge on the SPACE button
void AMainCharacter::DodgeInput()
{
	if (CanPerformDodge())
	{
		PerformDodge(DodgeCount, false);
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

void AMainCharacter::Jump()
{
	if (CanPerformJump())
	{
		if (StateManagerComponent && CombatComponent)
		{
			StopAnimMontage();
			StateManagerComponent->ResetState();
			CombatComponent->ResetAttack();

			Super::Jump();
		}
	}
}

// Function for delegate FOnStateBeginSignature
void AMainCharacter::OnStateBegin(const ECharacterState& CharacterState)
{
	switch (CharacterState)
	{
	case ECharacterState::None:
		break;
	case ECharacterState::GeneralActionState:
		break;
	case ECharacterState::Attacking:
		break;
	case ECharacterState::Dodging:
		break;
	case ECharacterState::Equipping:
		break;

	default:
		break;
	}
}

// Function for delegate FOnStateEndSignature
void AMainCharacter::OnStateEnd(const ECharacterState& CharacterState)
{
	switch (CharacterState)
	{
	case ECharacterState::None:
		break;
	case ECharacterState::GeneralActionState:
		break;
	case ECharacterState::Attacking:
		break;
	case ECharacterState::Dodging:
		break;
	case ECharacterState::Equipping:
		break;

	default:
		break;
	}
}

bool AMainCharacter::CanPerformAttack()
{
	if (StateManagerComponent)
	{
		UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			TArray<ECharacterState> StatesToCheckArray{
				ECharacterState::GeneralActionState,
				ECharacterState::Attacking,
				ECharacterState::Dodging,
				ECharacterState::Equipping };
			bool bIsCurrentStateEqualToAny = StateManagerComponent->IsCurrentStateEqualToAny(StatesToCheckArray);
			
			return !bIsCurrentStateEqualToAny && !CharacterMovementComponent->IsFalling();
		}
	}

	return false;
}

bool AMainCharacter::CanPerformDodge()
{
	if (StateManagerComponent)
	{
		UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			TArray<ECharacterState> StatesToCheckArray{
				ECharacterState::GeneralActionState,
				ECharacterState::Dodging,
				ECharacterState::Equipping };
			bool bIsCurrentStateEqualToAny = StateManagerComponent->IsCurrentStateEqualToAny(StatesToCheckArray);

			return !bIsCurrentStateEqualToAny && !CharacterMovementComponent->IsFalling();
		}
	}

	return false;
}

void AMainCharacter::Attack()
{
	if (CanPerformAttack())
	{
		PerformAttack(ECharacterAction::LightAttack, CombatComponent->AttackCount, false);
	}
}

bool AMainCharacter::CanPerformEquip()
{
	if (StateManagerComponent)
	{
		UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			if (CombatComponent)
			{
				TArray<ECharacterState> StatesToCheckArray{
					ECharacterState::GeneralActionState,
					ECharacterState::Attacking,
					ECharacterState::Dodging,
					ECharacterState::Equipping };
				bool bIsCurrentStateEqualToAny = StateManagerComponent->IsCurrentStateEqualToAny(StatesToCheckArray);

				return !bIsCurrentStateEqualToAny && !CharacterMovementComponent->IsFalling()  && CombatComponent->GetMainWeapon();
			}
		}
	}

	return false;
}

bool AMainCharacter::CanPerformJump()
{
	if (StateManagerComponent)
	{
		UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			TArray<ECharacterState> StatesToCheckArray{
				ECharacterState::GeneralActionState,
				ECharacterState::Dodging,
				ECharacterState::Equipping };
			bool bIsCurrentStateEqualToAny = StateManagerComponent->IsCurrentStateEqualToAny(StatesToCheckArray);

			return !bIsCurrentStateEqualToAny && !CharacterMovementComponent->IsFalling();
		}
	}

	return false;
}

bool AMainCharacter::CanPerformCrouch()
{
	if (StateManagerComponent)
	{
		UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			TArray<ECharacterState> StatesToCheckArray{
				ECharacterState::GeneralActionState,
				ECharacterState::Dodging,
				ECharacterState::Equipping };
			bool bIsCurrentStateEqualToAny = StateManagerComponent->IsCurrentStateEqualToAny(StatesToCheckArray);

			return !bIsCurrentStateEqualToAny && !CharacterMovementComponent->IsFalling();
		}
	}

	return false;
}

void AMainCharacter::OnActionBegin(const ECharacterAction& CharacterAction)
{
	switch (CharacterAction)
	{
	case ECharacterAction::None:
		break;
	case ECharacterAction::GeneralAction:
		break;
	case ECharacterAction::LightAttack:
		break;
	case ECharacterAction::StrongAttack:
		break;
	case ECharacterAction::ChargedAttack:
		break;
	case ECharacterAction::FallingAttack:
		break;
	case ECharacterAction::Dodge:
		break;
	case ECharacterAction::EnterCombat:
		break;
	case ECharacterAction::ExitCombat:
		break;

	default:
		break;
	}
}

void AMainCharacter::OnActionEnd(const ECharacterAction& CharacterAction)
{
	switch (CharacterAction)
	{
	case ECharacterAction::None:
		break;
	case ECharacterAction::GeneralAction:
		break;
	case ECharacterAction::LightAttack:
		break;
	case ECharacterAction::StrongAttack:
		break;
	case ECharacterAction::ChargedAttack:
		break;
	case ECharacterAction::FallingAttack:
		break;
	case ECharacterAction::Dodge:
		break;
	case ECharacterAction::EnterCombat:
		break;
	case ECharacterAction::ExitCombat:
		break;

	default:
		break;
	}
}

void AMainCharacter::PerformAction(TArray<UAnimMontage*> ActionMontages, ECharacterAction CharacterAction, ECharacterState CharacterState, int32 MontageIndex, bool bRandomIndex)
{
	ABaseWeapon* MainWeapon = CombatComponent->GetMainWeapon();
	if (MainWeapon)
	{
		UAnimMontage* ActionMontage = nullptr;

		if (bRandomIndex)
		{
			MontageIndex = FMath::RandRange(0, ActionMontages.Num() - 1);
			ActionMontage = ActionMontages[MontageIndex];
		}
		else
		{
			ActionMontage = ActionMontages[MontageIndex];
		}

		if (ActionMontage)
		{
			if (StateManagerComponent)
			{
				StateManagerComponent->SetState(CharacterState);
				StateManagerComponent->SetCurrentAction(CharacterAction);

				PlayAnimMontage(ActionMontage);

				if (MontageIndex >= ActionMontages.Num())
				{
					MontageIndex = 0;
				}
			}
		}
	}
}

void AMainCharacter::SprintInput()
{
	SetMovementSpeedMode(EMovementSpeedMode::Sprinting);
}

void AMainCharacter::StopSprintInput()
{
	if (GetCurrentMovementSpeedMode() == EMovementSpeedMode::Sprinting)
	{
		SetMovementSpeedMode(EMovementSpeedMode::Jogging);
	}
}

void AMainCharacter::SetMovementSpeedMode(EMovementSpeedMode NewSpeedMode)
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent)
	{
		if (NewSpeedMode != CurrentMovementSpeedMode)
		{
			CurrentMovementSpeedMode = NewSpeedMode;

			switch (CurrentMovementSpeedMode)
			{
			case EMovementSpeedMode::Walking:
				CharacterMovementComponent->MaxWalkSpeed = WalkingSpeed;

				break;
			case EMovementSpeedMode::Jogging:
				CharacterMovementComponent->MaxWalkSpeed = JoggingSpeed;

				break;
			case EMovementSpeedMode::Sprinting:
				CharacterMovementComponent->MaxWalkSpeed = SprintingSpeed;

				break;

			default:
				break;
			}
		}
	}
}