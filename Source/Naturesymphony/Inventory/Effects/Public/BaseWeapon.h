// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Naturesymphony/Inventory/Effects/Public/EquipEffect.h"
#include "BaseWeapon.generated.h"

/**
 * 
 */
UCLASS()
class NATURESYMPHONY_API ABaseWeapon : public AEquipEffect
{
	GENERATED_BODY()

public:
	ABaseWeapon();

	// Function override for equipping weapons on a the character.
	virtual void OnEquipped(ECombatType CombatType) override;

	UFUNCTION(BlueprintCallable)
	TArray<UAnimMontage*> GetActionMontages(ECharacterAction CharacterAction);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName HandSocketName = FName();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UCollisionComponent> CollisionComponent;

	UPROPERTY(EditDefaultsOnly)
	float BaseDamage = 0.0f;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
	TObjectPtr<UAnimMontage> EnterCombat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
	TObjectPtr<UAnimMontage> ExitCombat = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
	TArray<TObjectPtr<UAnimMontage>> LightAttackMontageArray;

private:
	UFUNCTION()
	void OnHit(const FHitResult& HitResult);
};
