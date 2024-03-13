// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Called when the game starts
void AMainPlayerController::BeginPlay()
{
	UUserWidget* HUDScreen = CreateWidget(this, HUDScreenClass);
	if (HUDScreen != nullptr)
	{
		HUDScreen->AddToViewport();
	}
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		Input->BindAction(InventoryInputAction, ETriggerEvent::Started, this, &AMainPlayerController::OpenMenu);
	}
}

void AMainPlayerController::OpenMenu()
{
	UUserWidget* InventoryMenu = CreateWidget(this, InventoryMenuClass);
	if (InventoryMenu != nullptr)
	{
		InventoryMenu->AddToViewport();
	}
}