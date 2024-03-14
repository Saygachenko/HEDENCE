// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFrameWork/Character.h"

// Called when the game starts
void AMainPlayerController::BeginPlay()
{
	HUDScreen = CreateWidget(this, HUDScreenClass);
	if (HUDScreen != nullptr)
	{
		HUDScreen->AddToViewport();
	}
}

// Function input Controller
void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		Input->BindAction(InventoryInputAction, ETriggerEvent::Started, this, &AMainPlayerController::OpenMenu);
	}
}

// Function to open menu inventory
void AMainPlayerController::OpenMenu()
{
	if (IsLocalPlayerController())
	{
		InventoryMenu = CreateWidget(this, InventoryMenuClass);
		if (InventoryMenu != nullptr)
		{
			InventoryMenu->AddToViewport();
		}
	}
}