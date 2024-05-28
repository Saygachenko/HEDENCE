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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
	TArray<UAnimMontage*> AttackMontageArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
	TArray<UAnimMontage*> DodgeMontageArray;

	int32 DodgeCount = 0;

	ABaseWeapon();

	// Function override for equipping weapons on a the character.
	virtual void OnEquipped(ECombatType CombatType) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName HandSocketName = FName();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UCollisionComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly)
	float BaseDamage = 0.0f;

	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnHit(const FHitResult& HitResult);
};
