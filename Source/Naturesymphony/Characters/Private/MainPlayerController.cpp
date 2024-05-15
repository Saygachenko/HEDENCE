// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Characters/Public/MainPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFrameWork/Character.h"

// Called when the game starts
void AMainPlayerController::BeginPlay()
{
	HUDScreen = CreateWidget(this, HUDScreenClass);
	if (HUDScreen)
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
		Input->BindAction(InventoryInputAction, ETriggerEvent::Started, this, &AMainPlayerController::OpenInventoryMenu);
		Input->BindAction(MenuInputAction, ETriggerEvent::Started, this, &AMainPlayerController::OpenMenu);
	}
}

// Function to open menu inventory
void AMainPlayerController::OpenInventoryMenu()
{
	InventoryMenu = CreateWidget(this, InventoryMenuClass);
	if (InventoryMenu != nullptr)
	{
		InventoryMenu->AddToViewport();
	}
}

// Function to open menu
void AMainPlayerController::OpenMenu()
{
	if (MenuClass)
	{
		if (Menu)
		{
			if (!Menu->IsInViewport())
			{
				Menu->AddToViewport();
			}
		}
		else
		{
			Menu = CreateWidget(this, MenuClass);
			if (Menu)
			{
				Menu->AddToViewport();
			}
		}
	}
}