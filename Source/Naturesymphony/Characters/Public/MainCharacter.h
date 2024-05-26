// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Naturesymphony/Characters/Public/CombatInterface.h"
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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UInventorySystemComponent* InventorySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USphereComponent* CameraCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UCombatComponent* CombatComponent = nullptr;

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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Function falling on ground from height and take damage
	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);

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

	// Function delegate death Character
	UFUNCTION()
	void OnDeath();

	UFUNCTION(BlueprintCallable)
	void PerformAttack(int32 AttackIndex, bool bRandomIndex);

private:
	bool bIsDodging = false;

	// Function movement for character
	void Move(const FInputActionValue& Value);

	// Function rotation camera
	void Look(const FInputActionValue& Value);

	// Function started walk for character
	void StartWalkMovement();

	// Function stoped walk for character
	void StopWalkMovement();

	// Function started crouch for character
	virtual void Crouch(bool bClientSimulation) override;

	// Function delegate OnComponentBeginOverlap
	UFUNCTION()
	void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Function delegate OnComponentEndOverlap
	UFUNCTION()
	void OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Function checked overlap camer and capsule of character
	void CheckCameraOverlap();

	// Function checks the conditions of the variables for weapon availability and weapon-specific combat type, and triggers the animation when the button is pressed.
	void OnEquipedItem();

	// Function for inputting a weapon attack to the LKM button
	void LightAttack();

	void PerformDodge(int32 DodgeIndex, bool bRandomIndex);

	// Function for inputtings a Character dodge on the SPACE button
	void Dodge();

	virtual void Jump() override;
};