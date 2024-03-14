// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UUserWidget;
class UInputAction;

/**
 * 
 */
UCLASS()
class NATURESYMPHONY_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// Function input Controller
	virtual void SetupInputComponent() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* InventoryInputAction;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HUDScreenClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> InventoryMenuClass;

	UPROPERTY()
	UUserWidget* HUDScreen = nullptr;

	UPROPERTY()
	UUserWidget* InventoryMenu = nullptr;

	// Function to open menu inventory
	void OpenMenu();
};
