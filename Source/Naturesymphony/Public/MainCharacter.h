// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "MainCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

UCLASS()
class NATURESYMPHONY_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent;

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

	// Function spawn weapon
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TSubclassOf<class ABaseWeaponActor> WeaponClass;

private:
	// Function movement for character
	void Move(const FInputActionValue& Value);

	// Function rotation camera
	void Look(const FInputActionValue& Value);

	// Function started walk for character
	void StartWalkMovement();

	// Function stoped walk for character
	void StopWalkMovement();

	// Function started crouch for character
	void StartCrouchMovement();

	// Function stoped crouch for character 
	void StopCrouchMovement();

	void SpawnWeapon();
};
