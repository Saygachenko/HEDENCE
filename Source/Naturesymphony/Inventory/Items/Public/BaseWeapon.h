// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Naturesymphony/Inventory/Items/Public/InventoryItem.h"
#include "BaseWeapon.generated.h"

/**
 * 
 */
UCLASS()
class NATURESYMPHONY_API ABaseWeapon : public AInventoryItem
{
	GENERATED_BODY()

public:
	ABaseWeapon();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UWidgetComponent* WidgetComponent;
};
