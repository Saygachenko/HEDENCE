// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Naturesymphony/Inventory/Items/Effects/Public/EquipEffect.h"
#include "BaseWeapon.generated.h"

/**
 * 
 */
UCLASS()
class NATURESYMPHONY_API ABaseWeapon : public AEquipEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* EnterCombat = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* ExitCombat = nullptr;

	// Function override for equipping weapons on a the character.
	virtual void OnEquipped(ACharacter* CharacterOwner, ECombatType CombatType) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName HandSocketName = FName();
};
