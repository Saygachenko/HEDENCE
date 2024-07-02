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
	TObjectPtr<UInputAction> InventoryInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	TObjectPtr<UInputAction> MenuInputAction;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HUDScreenClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> InventoryMenuClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MenuClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> InventoryMenu;

	UPROPERTY()
	TObjectPtr<UUserWidget> Menu = nullptr;

	// Function to open menu inventory
	void OpenInventoryMenu();

	// Function to open menu
	void OpenMenu();
};
