// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Naturesymphony/Characters/Public/CombatInterface.h"
#include "CharacterState.h"
#include "CharacterAction.h"
#include "MovementSpeedMode.h"
#include "MainCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

UCLASS()
class NATURESYMPHONY_API AMainCharacter : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this character's properties
	AMainCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ContinueAttack_Implementation() override;

	void ResetAttack_Implementation() override;

	FRotator GetDesiredRotation_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void SetMovementSpeedMode(EMovementSpeedMode NewSpeedMode);

	UFUNCTION(BlueprintPure)
	EMovementSpeedMode GetCurrentMovementSpeedMode() { return CurrentMovementSpeedMode; };

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UInventorySystemComponent* InventorySystemComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USphereComponent* CameraCollisionComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UCombatComponent* CombatComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStateManagerComponent* StateManagerComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* LookInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* JumpInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* WalkInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* CrouchInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* EquipWeaponInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* InteractInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* LightAttackInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* DodgeInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* SprintInputAction;

	// Min value for fall from height
	UPROPERTY(EditDefaultsOnly, Category = "Range|FallGround", meta=(ClampMin = 0.0f, ClampMax = 5000.0f))
	float MinVelocityZ = 800.0f;

	// Max value for fall from height
	UPROPERTY(EditDefaultsOnly, Category = "Range|FallGround", meta=(ClampMin = 0.0f, ClampMax = 5000.0f))
	float MaxVelocityZ = 1200.0f;

	// Min value damage for from fall
	UPROPERTY(EditDefaultsOnly, Category = "Range|FallGround", meta = (ClampMin = 0.0f, ClampMax = 1000.0f))
	float MinDamageLanded = 10.0f;

	// Max value damage for from fall
	UPROPERTY(EditDefaultsOnly, Category = "Range|FallGround", meta = (ClampMin = 0.0f, ClampMax = 1000.0f))
	float MaxDamageLanded = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
	TArray<UAnimMontage*> DodgeMontageArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MovementSpeed");
	float WalkingSpeed = 250.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MovementSpeed");
	float JoggingSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MovementSpeed");
	float SprintingSpeed = 800.0f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Function falling on ground from height and take damage
	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);

	// Function delegate death Character
	UFUNCTION()
	void OnDeath();

	UFUNCTION(BlueprintCallable)
	void PerformAttack(ECharacterAction AttackType, int32 AttackIndex, bool bRandomIndex);

private:
	UPROPERTY(EditDefaultsOnly, Category = "MovementSpeed");
	EMovementSpeedMode CurrentMovementSpeedMode = EMovementSpeedMode::Jogging;

	int32 DodgeCount = 0;

	// Function movement for character
	void Move(const FInputActionValue& Value);

	// Function rotation camera
	void Look(const FInputActionValue& Value);

	// Function started walk for character
	void WalkInput();

	// Function stoped walk for character
	void StopWalkInput();

	// Function started crouch for character
	virtual void Crouch(bool bClientSimulation) override;

	// Function for delegate OnComponentBeginOverlap
	UFUNCTION()
	void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Function for delegate OnComponentEndOverlap
	UFUNCTION()
	void OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Function checked overlap camer and capsule of character
	void CheckCameraOverlap();

	// Function checks the conditions of the variables for weapon availability and weapon-specific combat type, and triggers the animation when the button is pressed.
	void EquipInput();

	// Function for inputting a weapon attack to the LKM button
	void AttackInput();

	void PerformDodge(int32 DodgeIndex, bool bRandomIndex);

	// Function for inputtings a Character dodge on the SPACE button
	void DodgeInput();

	void Jump() override;

	// Function for delegate FOnStateBeginSignature
	UFUNCTION()
	void OnStateBegin(const ECharacterState& CharacterState);

	// Function for delegate FOnStateEndSignature
	UFUNCTION()
	void OnStateEnd(const ECharacterState& CharacterState);

	bool CanPerformAttack();

	bool CanPerformDodge();

	void Attack();

	bool CanPerformEquip();

	bool CanPerformJump();

	bool CanPerformCrouch();

	// Function for delegate FOnCharacterActionBeginSignature
	UFUNCTION()
	void OnActionBegin(const ECharacterAction& CharacterAction);

	// Function for delegate FOnCharacterActionEndSignature
	UFUNCTION()
	void OnActionEnd(const ECharacterAction& CharacterAction);

	UFUNCTION(BlueprintCallable)
	void PerformAction(TArray<UAnimMontage*> ActionMontages, ECharacterAction CharacterAction = ECharacterAction::GeneralAction, ECharacterState CharacterState = ECharacterState::GeneralActionState, int32 MontageIndex = 0, bool bRandomIndex = false);

	void SprintInput();

	void StopSprintInput();
};