// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Naturesymphony/Inventory/Items/Effects/Public/ItemEffect.h"
#include "EquipWeaponEffect.generated.h"

/**
 * 
 */
UCLASS()
class NATURESYMPHONY_API AEquipWeaponEffect : public AItemEffect
{
	GENERATED_BODY()

public:
	AEquipWeaponEffect();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* SkeletalMeshComponent;

private:
	// Function spawn weapons
	UFUNCTION()
	void EquipWeaponToHip();
};
