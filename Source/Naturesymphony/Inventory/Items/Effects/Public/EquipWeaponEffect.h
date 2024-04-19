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

	void SetAttachedToHand(bool IsAttachedToHand);

	UFUNCTION(BlueprintPure)
	bool IsAttachedToHand() { return IsAttachedToHands; };

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* DataTable = nullptr;

private:
	bool IsAttachedToHands = false;

	UPROPERTY()
	class UMainPlayerAnimInstance* MainPlayerAnimInstance = nullptr;

	// Function spawn weapons
	void EquipWeaponToHip();
};
