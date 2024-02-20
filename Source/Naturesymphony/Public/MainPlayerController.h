// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class NATURESYMPHONY_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadWrite)
	class UHealthComponent* HealthComponentController;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HUDScreenClass;

};
