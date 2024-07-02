// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickUpMessageWidget.generated.h"

UCLASS()
class NATURESYMPHONY_API UPickUpMessageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Message;

	// Function show message of widget
	void ShowMessage(const FText& Text);
};
