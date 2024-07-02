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
	int32 AttackCount = 0;

	// Sets default values for this component's properties
	UCombatComponent();

	void SetMainWeapon(ABaseWeapon* NewWeapon);

	UFUNCTION(BlueprintPure)
	ABaseWeapon* GetMainWeapon() { return MainWeapon; };

	UFUNCTION(BlueprintCallable)
	void SetCombatEnabled(bool CombatEnabled);

	UFUNCTION(BlueprintPure)
	bool GetCombatEnabled() { return bCombatEnabled; };

	void SetIsAttackSaved(bool bIsAttackedSaved) { bIsAttackSaved = bIsAttackedSaved; };

	UFUNCTION(BlueprintPure)
	bool GetIsAttackSaved() { return bIsAttackSaved; };

	void ResetAttack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<ABaseWeapon> MainWeapon;

	bool bIsAttackSaved = false;

	bool bCombatEnabled = false;

};
