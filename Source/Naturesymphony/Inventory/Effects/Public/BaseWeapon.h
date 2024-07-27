// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Naturesymphony/Inventory/Effects/Public/EquipEffect.h"
#include "Naturesymphony/Components/Characters/Public/StateManagerComponent.h"
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

	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetStatCostForAction() { return OwnerStateManager ? ActionStatCost.FindRef(OwnerStateManager->GetCurrentAction()) : 0.0f; };

	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetDamage() { return OwnerStateManager ? FMath::Clamp(ActionDamageMultiplier.FindRef(OwnerStateManager->GetCurrentAction()), 1.0f, ActionDamageMultiplier.FindRef(OwnerStateManager->GetCurrentAction())) * BaseDamage : BaseDamage; };

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName HandSocketName = FName();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UCollisionComponent> CollisionComponent;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
	TObjectPtr<UAnimMontage> EnterCombat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
	TObjectPtr<UAnimMontage> ExitCombat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
	TArray<TObjectPtr<UAnimMontage>> LightAttackMontageArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animations")
	TArray<TObjectPtr<UAnimMontage>> JumpAttackMontage;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float BaseDamage = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	TMap<ECharacterAction, float> ActionStatCost;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	TMap<ECharacterAction, float> ActionDamageMultiplier;

	UPROPERTY()
	TObjectPtr<UStateManagerComponent> OwnerStateManager;

	UFUNCTION()
	void OnHit(const FHitResult& HitResult);
};
