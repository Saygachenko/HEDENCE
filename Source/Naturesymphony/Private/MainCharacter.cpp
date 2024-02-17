// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

// Функция передвижения персонажа
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

// Функция поворота камеры
void AMainCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D AxisValue = Value.Get<FVector2D>();

	AddControllerPitchInput(AxisValue.Y);
	AddControllerYawInput(AxisValue.X);
}

// Функция начала ходьбы для персонажа
void AMainCharacter::StartWalkMovement()
{
	if(GetCharacterMovement())
	{ 
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

// Функция остановки ходьбы для персонажа
void AMainCharacter::StopWalkMovement()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
}

// Функция начала приседания для персонажа
void AMainCharacter::StartCrouchMovement()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	}
}

// Функция остановки приседания для персонажа
void AMainCharacter::StopCrouchMovement()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = false;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
}
