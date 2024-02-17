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
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* JumpInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* WalkInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* CrouchInputAction;

	// Переменная изменяет поле MaxWalkSpeed. Она так же работает и для скорости приседания.
	UPROPERTY(EditDefaultsOnly, Category = "Custom Character Movement")
	float WalkSpeed = 300.0f; 

private:

	// Функция передвижения персонажа
	void Move(const FInputActionValue& Value);

	// Функция поворота камеры
	void Look(const FInputActionValue& Value);

	// Функция начала ходьбы для персонажа
	void StartWalkMovement();

	// Функция остановки ходьбы для персонажа
	void StopWalkMovement();

	// Функция начала приседания для персонажа
	void StartCrouchMovement();

	// Функция остановки приседания для персонажа
	void StopCrouchMovement();
};
