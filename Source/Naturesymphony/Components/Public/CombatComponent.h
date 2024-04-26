// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NATURESYMPHONY_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

	void SetMainWeapon(ABaseWeapon* NewWeapon);

	UFUNCTION(BlueprintPure)
	ABaseWeapon* GetMainWeapon() { return MainWeapon; };

	UFUNCTION(BlueprintCallable)
	void SetCombatEnabled(bool CombatEnabled);

	UFUNCTION(BlueprintPure)
	bool GetCombatEnabled() { return bCombatEnabled; };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class ABaseWeapon* MainWeapon = nullptr;

	bool bCombatEnabled = false;
};
