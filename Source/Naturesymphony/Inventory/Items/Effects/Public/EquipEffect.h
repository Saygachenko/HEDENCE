// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Naturesymphony/Inventory/Items/Effects/Public/ItemEffect.h"
#include "EquipEffect.generated.h"

UCLASS()
class NATURESYMPHONY_API AEquipEffect : public AItemEffect
{
	GENERATED_BODY()

public:
	AEquipEffect();

	// Getter StaticMesh or SkeletalMesh
	UFUNCTION(BlueprintPure)
	UPrimitiveComponent* GetItemMesh();

	// Function checks IsEquipped item
	void OnUnequipped();

	// Function attach actor on a character
	UFUNCTION(BlueprintCallable)
	void AttachActor(ACharacter* CharacterOwner, FName SocketName);

	void SetIsEuqipped(bool IsEquipped) { bIsEquipped = IsEquipped; };

	UFUNCTION(BlueprintPure)
	bool GetIsEquipped() { return bIsEquipped; };

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapons")
	FName AttachSocketName = FName();

	virtual void BeginPlay() override;

	// Basic function of equipping item on a character
	UFUNCTION(BlueprintCallable)
	virtual void OnEquipped(ACharacter* CharacterOwner, ECombatType CombatType);

private:
	bool bIsEquipped = false;

};
