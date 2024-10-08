// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Naturesymphony/Structs/Inventory/Public/ItemDataStructs.h"
#include "AnimInstanceInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAnimInstanceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NATURESYMPHONY_API IAnimInstanceInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Function implemented in the blueprint class ABP
	UFUNCTION(BlueprintNativeEvent)
	void UpdateCombatType(ECombatType CombatType);

	// Function implemented in the blueprint class ABP
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateCombatEnabled(bool CombatEnabled);
};
