// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "Blueprint/UserWidget.h"

// Called when the game starts
void AMainPlayerController::BeginPlay()
{
	UUserWidget* HUDScreen = CreateWidget(this, HUDScreenClass);
	if (HUDScreen != nullptr)
	{
		HUDScreen->AddToViewport();
	}
}