// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySystemComponent.generated.h"

// Struct information about a slot in your inventory
USTRUCT()
struct FSlotStruct
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FName ID;

	UPROPERTY(EditDefaultsOnly)
	int32 Quantity;
};

// In the future, the entire component needs to be replicated for online
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NATURESYMPHONY_API UInventorySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventorySystemComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	// We will need to replicate an array of the structure
	UPROPERTY(EditDefaultsOnly, Category = "Invetory Data")
	TArray<FSlotStruct> SlotStruct;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Invetory Data")
	int32 InventorySize = 16;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Invetory Data")
	float SphereTraceRadius = 150.0f;

private:
	UPROPERTY()
	AActor* LookAtActor = nullptr;

	UPROPERTY()
	AActor* LastLookedActor = nullptr;

	// Function add to inventory
	void AddToInventory();

	// Function remove to inventory
	void RemoveFromInventory();

	// Function overlapping with item
	void InteractionTrace();
};
