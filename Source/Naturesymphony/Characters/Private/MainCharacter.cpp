// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Characters/Public/MainCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFrameWork/Character.h"
#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Naturesymphony/Components/Inventory/Public/InventorySystemComponent.h"
#include "Naturesymphony/Inventory/Effects/Public/BaseWeapon.h"
#include "Naturesymphony/Components/Combat/Public/CombatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Naturesymphony/Components/Characters/Public/StateManagerComponent.h"
#include "Naturesymphony/Enums/Characters/Public/CharacterState.h"
#include "Naturesymphony/Components/Stats/Public/StatsComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	StatsComponent = CreateDefaultSubobject<UStatsComponent>(FName("StatsComponent"));
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
	
	UWorld* World = GetWorld();
	if (World)
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				if (InputMapping)
				{
					InputSystem->AddMappingContext(InputMapping, 0);
				}
			}
		}
	}

	LandedDelegate.AddDynamic(this, &AMainCharacter::OnGroundLanded);
	CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnCameraCollisionBeginOverlap);
	CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::OnCameraCollisionEndOverlap);
	StateManagerComponent->OnStateBegin.AddDynamic(this, &AMainCharacter::OnStateBegin);
	StateManagerComponent->OnStateEnd.AddDynamic(this, &AMainCharacter::OnStateEnd);
	StateManagerComponent->OnCharacterActionBegin.AddDynamic(this, &AMainCharacter::OnActionBegin);
	StateManagerComponent->OnCharacterActionEnd.AddDynamic(this, &AMainCharacter::OnActionEnd);
	OnTakePointDamage.AddDynamic(this, &AMainCharacter::TakePointDamage);
	StatsComponent->OnCurrentStatValueUpdated.AddDynamic(this, &AMainCharacter::OnCurrentStatValueUpdate);

	StatsComponent->InitializeStats();
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
		Input->BindAction(JumpInputAction, ETriggerEvent::Completed, this, &AMainCharacter::Jump);
		Input->BindAction(WalkInputAction, ETriggerEvent::Started, this, &AMainCharacter::WalkInput);
		Input->BindAction(WalkInputAction, ETriggerEvent::Completed, this, &AMainCharacter::StopWalkInput);
		Input->BindAction(CrouchInputAction, ETriggerEvent::Started, this, &AMainCharacter::Crouch, false);
		Input->BindAction(CrouchInputAction, ETriggerEvent::Completed, this, &ACharacter::UnCrouch, false);
		Input->BindAction(InteractInputAction, ETriggerEvent::Started, this, &AMainCharacter::InventoryInput);
		Input->BindAction(EquipWeaponInputAction, ETriggerEvent::Started, this, &AMainCharacter::EquipInput);
		Input->BindAction(LightAttackInputAction, ETriggerEvent::Started, this, &AMainCharacter::AttackInput);
		Input->BindAction(LightAttackInputAction, ETriggerEvent::Completed, this, &AMainCharacter::StopAttackInput);
		Input->BindAction(DodgeInputAction, ETriggerEvent::Started, this, &AMainCharacter::DodgeInput);
		Input->BindAction(SprintInputAction, ETriggerEvent::Started, this, &AMainCharacter::SprintInput);
		Input->BindAction(SprintInputAction, ETriggerEvent::Completed, this, &AMainCharacter::StopSprintInput);
		Input->BindAction(JumpAttackInputAction, ETriggerEvent::Started, this, &AMainCharacter::JumpAttackInput);
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
		StatsComponent->TakeDamage(FinalDamage);
	}
}

void AMainCharacter::Death()
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
						PerformAction(ECharacterAction::EnterCombat);
					}
					else
					{
						PerformAction(ECharacterAction::ExitCombat);
					}
				}
			}
		}
	}
}

void AMainCharacter::PerformAttack(ECharacterAction AttackType, int32& AttackIndex, bool bRandomIndex)
{
	if (CombatComponent)
	{
		ABaseWeapon* MainWeapon = CombatComponent->GetMainWeapon();
		if (MainWeapon && CombatComponent->GetCombatEnabled())
		{
			UAnimMontage* AttackMontage = nullptr;
			TArray<UAnimMontage*> AttackMontages = MainWeapon->GetActionMontages(AttackType);

			if (!AttackMontages.IsEmpty())
			{
				if (bRandomIndex)
				{
					AttackIndex = FMath::RandRange(0, AttackMontages.Num() - 1);
				}
				else
				{
					if (AttackIndex >= AttackMontages.Num())
					{
						AttackIndex = 0;
					}
				}
			}

			AttackMontage = AttackMontages[AttackIndex];
			if(AttackMontage)
			{
				StateManagerComponent->SetState(ECharacterState::Attacking);
				StateManagerComponent->SetCurrentAction(AttackType);

				PlayAnimMontage(AttackMontage);

				AttackIndex++;
				if (AttackIndex >= AttackMontages.Num())
				{
					AttackIndex = 0;
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

void AMainCharacter::PerformDodge(int32& DodgeIndex, bool bRandomIndex)
{
	if (!DodgeMontageArray.IsEmpty())
	{
		UAnimMontage* DodgeMontage = nullptr;

		if (bRandomIndex)
		{
			DodgeIndex = FMath::RandRange(0, DodgeMontageArray.Num() - 1);
		}

		DodgeMontage = DodgeMontageArray[DodgeIndex];
		if (DodgeMontage)
		{
			if (StateManagerComponent)
			{
				StateManagerComponent->SetState(ECharacterState::Dodging);

				PlayAnimMontage(DodgeMontage);

				DodgeIndex++;
				if (DodgeIndex >= DodgeMontageArray.Num())
				{
					DodgeIndex = 0;
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
		if (StatsComponent)
		{
			PerformDodge(DodgeCount, false);
			StatsComponent->ModifyCurrentStatValue(EStats::Stamina, -30.0f, true);
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

void AMainCharacter::Jump()
{
	if (CanPerformJump())
	{
		if (StateManagerComponent && CombatComponent && !JumpAttack)
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
	case ECharacterState::Dead:

		Death();
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
	case ECharacterState::Dead:
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
				ECharacterState::Equipping,
				ECharacterState::Dead };
			bool bIsCurrentStateEqualToAny = StateManagerComponent->IsCurrentStateEqualToAny(StatesToCheckArray);
			
			return !bIsCurrentStateEqualToAny && !CharacterMovementComponent->IsFalling() && CombatComponent->GetCombatEnabled() && StatsComponent->GetCurrentStatValue(EStats::Stamina) >= 10.0f;
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
				ECharacterState::Equipping,
				ECharacterState::Dead };
			bool bIsCurrentStateEqualToAny = StateManagerComponent->IsCurrentStateEqualToAny(StatesToCheckArray);

			return !bIsCurrentStateEqualToAny && !CharacterMovementComponent->IsFalling() && StatsComponent->GetCurrentStatValue(EStats::Stamina) >= 10.0f;
		}
	}

	return false;
}

void AMainCharacter::Attack()
{
	if (CanPerformAttack())
	{
		if (StatsComponent)
		{
			if (JumpAttack)
			{
				PerformAttack(ECharacterAction::JumpAttack, CombatComponent->AttackCount, false);
				StatsComponent->ModifyCurrentStatValue(EStats::Stamina, -25.0f, true);
			}
			else
			{
				PerformAttack(ECharacterAction::LightAttack, CombatComponent->AttackCount, false);
				StatsComponent->ModifyCurrentStatValue(EStats::Stamina, -15.0f, true);
			}
		}
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
					ECharacterState::Equipping,
					ECharacterState::Dead };
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
				ECharacterState::Attacking,
				ECharacterState::Dodging,
				ECharacterState::Equipping,
				ECharacterState::Dead };
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
				ECharacterState::Equipping,
				ECharacterState::Dead };
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
	case ECharacterAction::EnterCombat:
		break;
	case ECharacterAction::ExitCombat:
		break;

	default:
		break;
	}
}

void AMainCharacter::PerformAction(ECharacterAction CharacterAction, ECharacterState CharacterState, int32 MontageIndex, bool bRandomIndex)
{
	ABaseWeapon* MainWeapon = CombatComponent->GetMainWeapon();
	if (MainWeapon)
	{
		UAnimMontage* ActionMontage = nullptr;
		TArray<UAnimMontage*> ActionMontages = MainWeapon->GetActionMontages(CharacterAction);

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
	UWorld* World = GetWorld();
	if (World)
	{
		if (CanPerformSprint())
		{
			SetMovementSpeedMode(EMovementSpeedMode::Sprinting);
			World->GetTimerManager().SetTimer(SprintStaminaCostTimer, this, &AMainCharacter::SprintStaminaCost, 0.1f, true);
		}
	}
}

void AMainCharacter::StopSprintInput()
{
	DisableSprint();
}

void AMainCharacter::InventoryInput()
{
	if (InventorySystemComponent)
	{
		InventorySystemComponent->Interact();
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

void AMainCharacter::JumpAttackInput()
{
	JumpAttack = true;
}

void AMainCharacter::StopAttackInput()
{
	JumpAttack = false;
}

void AMainCharacter::TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("DamagedActor: %s, Damage: %f, DamageCauser: %s, InstigatedBy: %s"), *DamagedActor->GetName(), Damage, *DamageCauser->GetName(), *InstigatedBy->GetName());
	if (StatsComponent)
	{
		StatsComponent->TakeDamage(Damage);

		if (InpactResponce)
		{
			PlayAnimMontage(InpactResponce);
		}
	}
}

void AMainCharacter::OnCurrentStatValueUpdate(EStats Stat, float Value)
{
	if (Stat == EStats::Health && Value <= 0.0f)
	{
		if (StateManagerComponent)
		{
			StateManagerComponent->SetState(ECharacterState::Dead);
		}
	}
}

void AMainCharacter::SprintStaminaCost()
{
	if (CanPerformSprint())
	{
		if (StatsComponent)
		{
			StatsComponent->ModifyCurrentStatValue(EStats::Stamina, -2.0f, true);
			
			if (StatsComponent->GetCurrentStatValue(EStats::Stamina) < 10.0f)
			{
				DisableSprint();
			}
		}
	}
	else
	{
		DisableSprint();
	}
}

void AMainCharacter::DisableSprint()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(SprintStaminaCostTimer);

		if (GetCurrentMovementSpeedMode() == EMovementSpeedMode::Sprinting)
		{
			SetMovementSpeedMode(EMovementSpeedMode::Jogging);
		}
	}
}

bool AMainCharacter::CanPerformSprint()
{
	return GetVelocity().Length() != 0.0f;
}