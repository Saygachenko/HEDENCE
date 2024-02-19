// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

	// Variable range fall from height
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector2D LandedVelocity = FVector2D(800.0f, 1200.0f);

	// Variable range damage from height
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

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

};
