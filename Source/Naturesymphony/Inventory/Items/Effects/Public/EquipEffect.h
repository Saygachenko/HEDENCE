// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Naturesymphony/Inventory/Items/Effects/Public/ItemEffect.h"
#include "EquipEffect.generated.h"

/**
 * 
 */
UCLASS()
class NATURESYMPHONY_API AEquipEffect : public AItemEffect
{
	GENERATED_BODY()

public:
	AEquipEffect();

	// Setter for IsAttachedToHand
	void SetAttachedToHand(bool IsAttachedToHand);

	// Getter for IsAttachedToHand
	UFUNCTION(BlueprintPure)
	bool GetAttachedToHand() { return IsAttachedToHands; };

protected:
	virtual void BeginPlay() override;

private:
	bool IsAttachedToHands = false;

	UPROPERTY()
	class USkeletalMeshComponent* MainCharacterMesh = nullptr;

	// Function first equip item on player
	void FirstEquipItem();
};
